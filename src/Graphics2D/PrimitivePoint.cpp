#include <Graphics2D/PrimitivePoint.hh>

namespace Graphics2D {

  PrimitivePoint::PrimitivePoint() {
    // default to a black point at (0,0)
    color_ = Color::black();
    coords_.resize(1);
    name_ = "Point";
  }

  PrimitivePoint::~PrimitivePoint() {
    // TODO Auto-generated destructor stub
  }

  void PrimitivePoint::SetCoordinate(const Coordinate &coo) {
    coords_[0] = coo;
  }

  void PrimitivePoint::Draw(ImageBase *im) {
    unsigned int w=im->GetWidth();
    unsigned int h=im->GetHeight();
    // check if point is in image
    if (coords_[0].x()>=0 && coords_[0].y()>=0 && coords_[0].x()<(int)w && coords_[0].y()<(int)h) {
      unsigned char *data = im->GetData();
      unsigned int idx = (coords_[0].y()*w+coords_[0].x())*3;
      // draw
      data[ idx + 0 ] = color_.r();
      data[ idx + 1 ] = color_.g();
      data[ idx + 2 ] = color_.b();
    }
  }
}
