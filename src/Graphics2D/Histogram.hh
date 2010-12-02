#ifndef HISTOGRAM_HH
#define HISTOGRAM_HH

#include <vector>
#include <Graphics2D/Image.hh>
#include <Graphics2D/PrimitiveBox.hh>

namespace Graphics2D {
  class Histogram: public PrimitiveBase {
  public:
    Histogram(const Color& color, const Coordinate& pos, const Coordinate& size);
    void FromImage(const Image& image, const int& channel);
  
    int GetMin();
    int GetMax();
    
    void Autocontrast(const Image& src, Image& dst);
    
    virtual void SetCoordinates(const std::vector<Coordinate>& coordinates);
    
    void Draw(ImageBase* im);
  protected:
    void UpdateBoxes();
    
    std::vector<int> values;
    std::vector<PrimitiveBox> boxes;
  };
}

#endif
