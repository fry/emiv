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
    Image borders;
    Filter::GradMag(input, borders);
    
    const int width = input.GetWidth();
    const int height = input.GetHeight();
    const float center_x = width / 2.0;
    const float center_y = height / 2.0;
    const float max_d = std::sqrt(width*width + height*height);
    const int size_deg = static_cast<int>(180 * resolution);
    const int size_d = rint(max_d);
    
    houghspace_.Init(size_deg, size_d);
    
    for (int x=0; x < borders.GetWidth(); x++) {
      for (int y=0; y < borders.GetHeight(); y++) {
        const int magn = borders.GetPixel(x, y, 0);
        if (magn > 100) {
          for (int step=0; step < size_deg; step++) {
            const float deg = step / (float)resolution * M_PI / 180;
            std::cout << (float)std::cos(deg) * (x - center_x) + (float)std::sin(deg) * (y - center_y) + max_d/2 << std::endl;
            const int d = rint((float)std::cos(deg) * (x - center_x) + (float)std::sin(deg) * (y - center_y) + max_d/2);
            float new_val = houghspace_.GetPixel(step, d);
            new_val += 1;
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
    // get elements of structure tensor  (extend StructureTensor class for getting Jxx, Jxy, Jyy)
    // calculate hough transform using structure tensor orientation
    // consider needed size for 2d histogram!
    // save hough image for debugging
  }

  
  void HoughTransform::GetLines_(int imWidth, int imHeight, std::vector<PrimitiveLine> &lines) {
    // for each pixel != 0 in hough space
    // calculate a line from one image border to another
    // by calculating the intersection points with the image borders
    // consider vertical/horizontal lines, you might need to consider different cases
  }
}
