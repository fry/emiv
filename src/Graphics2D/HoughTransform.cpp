#include <Graphics2D/HoughTransform.hh>

using namespace std;

namespace Graphics2D {

  HoughTransform::HoughTransform() {
    // nothing
  }

  HoughTransform::~HoughTransform() {
    // nothing
  }

  
  void HoughTransform::StandardHoughTransform(const Image &input, const int resolution, vector<PrimitiveLine> &lines) {
    Create2DHistogram_(input, resolution);
    Filter::NonMaximumSuppression(houghspace_, houghspace_, 254);
    Image out(input);
    /*houghspace_.GetAsGreyImage(out);
    out.SavePPM("houghspace.ppm");*/
    GetLines_(input.GetWidth(), input.GetHeight(), lines);
    for(std::vector<PrimitiveLine>::iterator iter=lines.begin(); iter != lines.end(); ++iter) {
      iter->SetColor(Color::red());
      iter->Draw(&out);
    }
    out.SavePPM("test.ppm");
    // call Create2DHistogram_
    // apply non maximum suppression
    // save houghimage for debugging (optional)
    // call GetLines_
  }

  void HoughTransform::FastHoughTransform(const StructureTensor &input, const int resolution, vector<PrimitiveLine> &lines) {
    // call Create2DHistogramFromStructureTensor_
    // apply non maximum suppression
    // save hough image for debugging (optional)
    // call GetLines_ (extend StructureTensor class for getting width and height)
  }

  // ---- protected
  
  void HoughTransform::Create2DHistogram_(const Image &input, const int resolution) {
    resolution_ = resolution;
    
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
    const int size_deg = static_cast<int>(180 * resolution);

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
  }
  
  void HoughTransform::Create2DHistogramFromStructureTensor_(const StructureTensor &input, const int resolution) {
    // get elements of structure tensor  (extend StructureTensor class for getting Jxx, Jxy, Jyy)
    // calculate hough transform using structure tensor orientation
    // consider needed size for 2d histogram!
    // save hough image for debugging
  }

  
  void HoughTransform::GetLines_(int imWidth, int imHeight, std::vector<PrimitiveLine> &lines) {
    std::vector<Coordinate> points;
    const int max_d = houghspace_.GetHeight();
    for (int deg = 0; deg < houghspace_.GetWidth(); deg++) {
      for (int dist = 0; dist < max_d; dist++) {
        const float val = houghspace_.GetPixel(deg, dist);
        if (val > 0 && deg != 0) {
          std::cout << dist << std::endl;
          const float degrees = deg / (resolution_ * 180.0f) * M_PI;
          const float m = - std::cos(degrees) / std::sin(degrees);
          const float b = dist / std::sin(degrees);
          
          PrimitiveLine line;
          points.clear();
          points.push_back(Coordinate(0, b));
          points.push_back(Coordinate(imWidth, m * imWidth + b));
          line.SetCoordinates(points);
          lines.push_back(line);
        }
      }
    }
    // for each pixel != 0 in hough space 
    // calculate a line from one image border to another
    // by calculating the intersection points with the image borders
    // consider vertical/horizontal lines, you might need to consider different cases
  }
}
