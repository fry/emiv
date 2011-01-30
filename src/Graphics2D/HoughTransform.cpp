#include <Graphics2D/HoughTransform.hh>
#include <cmath>

using namespace std;

namespace Graphics2D {

  HoughTransform::HoughTransform() {
    // nothing
  }

  HoughTransform::~HoughTransform() {
    // nothing
  }

  
  void HoughTransform::StandardHoughTransform(const Image &input, const int resolution, vector<PrimitiveLine> &lines) {
    resolution_ = resolution;
    Create2DHistogram_(input, resolution);
    Filter::NonMaximumSuppression(houghspace_, houghspace_, 254);
    GetLines_(input.GetWidth(), input.GetHeight(), lines);
  }

  void HoughTransform::FastHoughTransform(const StructureTensor &input, const int resolution, vector<PrimitiveLine> &lines) {
    resolution_ = resolution;
    // call Create2DHistogramFromStructureTensor_
  	Create2DHistogramFromStructureTensor_(input, resolution);
    // apply non maximum suppression
  	Filter::NonMaximumSuppression(houghspace_, houghspace_, 20);
    GetLines_(input.GetWidth(), input.GetHeight(), lines);
   }

  // ---- protected
  
  void HoughTransform::Create2DHistogram_(const Image &input, const int resolution) {
    houghspace_.FillZero();
    
    Image borders;
    // filter input image to detect borders
    Filter::GradMag(input, borders);
    
    const int width = input.GetWidth();
    const int height = input.GetHeight();
    const float center_x = width / 2.0;
    const float center_y = height / 2.0;
    // calculate the maximum distance of a line from the center (half_d)
    const int max_d = rint(std::sqrt(width*width + height*height));
    const int half_d = max_d / 2;
    // resolution of degrees
    const int size_deg = 180 * resolution;

    // size of houghspace
    houghspace_.Init(size_deg, max_d);

    const float deg_factor = 1.0f / resolution * M_PI / 180.0f;
    for (int x=0; x < width; x++) {
      for (int y=0; y < height; y++) {
        // only consider borders
        const int magn = borders.GetPixel(x, y, 0);
        if (magn > 0) {
          // draw a line in houghspace for every possible line through this point
          for (int step=0; step < size_deg; step++) {
            // calculate degrees from discrete degree step
            const float deg = step * deg_factor;
            // calculate line distance from center and translate it into a positive area
            const int d = rint(std::cos(deg) * (x - center_x) + std::sin(deg) * (y - center_y)) + half_d;
            // increase amount of lines by one
            const float new_val = houghspace_.GetPixel(step, d) + 1;
            houghspace_.SetPixel(step, d, new_val);
          }
        }
      }
    }
    
    Image out;
    houghspace_.GetAsGreyImage(out);
    out.SavePPM("houghspace.ppm");
  }
  
  void HoughTransform::Create2DHistogramFromStructureTensor_(const StructureTensor &input, const int resolution) {
    houghspace_.FillZero();
    
    // get elements of structure tensor
	  FloatImage Jxx,Jxy,Jyy;
  	input.GetStructureTensor(Jxx,Jxy,Jyy);
  	//calculate needed size of houghspace_
  	const int width = input.GetWidth();
    const int height = input.GetHeight();
	  const float center_x = width / 2.0f;
    const float center_y = height / 2.0f;
	  const int size_deg = 180 * resolution;
  	const int max_d = rint(std::sqrt(width*width + height*height));
  	const int half_d = max_d/2;
    houghspace_.Init(size_deg, max_d);
    double min=0,max=0;
  	// calculate hough transform using structure tensor orientation
  	for (int x=0; x < width; x++) {
    	for (int y=0; y < height; y++) {
    		float jxx = Jxx.GetPixel(x,y);
    		float jxy = Jxy.GetPixel(x,y);
    		float jyy = Jyy.GetPixel(x,y);
        // check if we have an edge by using the trace
    		if ( (jxx + jyy) > 0) { 				
    			// calculate degrees from orientation vector, combine both 180 degree halfs (abs)
    			const double deg = std::abs(atan2(jyy-jxx, 2*jxy));
    			//calculate distance (as above)
    			const int d = rint(std::cos(deg) * (x - center_x) + std::sin(deg) * (y - center_y)) + half_d;
    			//calculate the bucket to choose
    			int bucket = static_cast<int>(deg*180*resolution/M_PI);
    			// increase amount of lines by one
    			const float new_val = houghspace_.GetPixel(bucket, d) + 1;
    			houghspace_.SetPixel(bucket, d, new_val);
  		  } 
    	}
    }
    
    Image out;
    houghspace_.GetAsGreyImage(out);
    out.SavePPM("houghspace_st.ppm");
  }

  
  void HoughTransform::GetLines_(int imWidth, int imHeight, std::vector<PrimitiveLine> &lines) {
    std::vector<Coordinate> points; points.reserve(2);
    const int max_d = rint(std::sqrt(imWidth * imWidth + imHeight * imHeight));
    
    for (int deg = 0; deg < houghspace_.GetWidth(); deg++) {
      for (int dist = 0; dist < houghspace_.GetHeight(); dist++) {
        const float val = houghspace_.GetPixel(deg, dist);
        if (val > 0) {
          points.clear();
          // convert degrees to radian
          const float degrees = deg / (resolution_ * 180.0f) * M_PI;
          // line dist is encoded in positive numbers, decode it
          const float real_d = dist - max_d/2;
          std::cout << "line: " << real_d << "," << (float)deg/resolution_ << std::endl;
          // first consider non-vertical lines (degrees != 0° && != 90°)
          if (deg != 0 && deg != 180 * resolution_) {
            // calculate inclination of line
            float m = - std::cos(degrees) / std::sin(degrees);
            // the hough space image we get is built on an image with its origin 
            // in the center, so we have to translate everything accordingly
            // calculate y axis cross section and translate to top left origin
            float b =  real_d / std::sin(degrees) + imHeight/2;
            // handle horizontal lines (sin(0°) == 0, so m = -inf)
            if (deg == 90 * resolution_)
              m = 0;

            // the y axis cross section is with the y axis in the middle of the screen
            // so we have to go back half the width to get the y position of the line
            // in our top left system
            points.push_back(Coordinate(0, b + -(imWidth/2) * m));
            points.push_back(Coordinate(imWidth, b + (imWidth/2) * m));
          } else {
            // handle vertical lines, translate them to the correct coordinate system
            points.push_back(Coordinate(real_d + imWidth/2, 0));
            points.push_back(Coordinate(real_d + imWidth/2, imHeight));
          }
          
          PrimitiveLine line;

          line.SetCoordinates(points);
          lines.push_back(line);
        }
      }
    }
  }
}
