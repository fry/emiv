/*
 * 1D Histogram calculated from an image
 * @author fkellner 10/10
 */
#ifndef HISTOGRAM_HH_
#define HISTOGRAM_HH_
#include <vector>
#include <Graphics2D/Image.hh>
#include <Graphics2D/Coordinate.hh>
#include <Graphics2D/PrimitiveBase.hh>
#include <Graphics2D/PrimitiveBox.hh>

namespace Graphics2D {

  class Histogram : public PrimitiveBase {
    public:
      Histogram();
      virtual ~Histogram();
      
      // creates histogram from given channel of given image
      int FromImage(const Image &image, const int &channel);
      
      // uses v histograms (from hsv color space) to maximize color contrast
      int Autocontrast(const Image &src, Image &dst);
      int AutocontrastWrong(const Image &src, Image &dst);
      
      virtual void SetCoordinates(std::vector<Coordinate> coo);
      virtual void Draw(ImageBase *im);
      
    protected:
      // get affine function to spread histogram
      int GetAutocontrastFunc_(float &m, float &b);
      
      // histogram
      std::vector<int> histo_;
      
    private:
      // keep memory for repeated usage
      Image hsvImage_;
      
      int width_, height_, posx_, posy_;
      std::vector<PrimitiveBox> boxes_;
  };

}

#endif /* HISTOGRAM_HH_ */
