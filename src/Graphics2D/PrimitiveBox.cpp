#include <Graphics2D/PrimitiveBox.hh>

namespace Graphics2D {

  PrimitiveBox::PrimitiveBox() {
    color_ = Color::black();
    lines_.resize(4);
    coords_.resize(5);
    name_ = "Box";
    closed_ = true;
  }
    
  PrimitiveBox::~PrimitiveBox() {
    // TODO Auto-generated destructor stub
  }

  void PrimitiveBox::SetCoordinates(const std::vector<Coordinate> &coords) {
    if (coords.size()==2) {
      std::vector<Coordinate> coo(5);
      int xs = coords[0].x();
      int ys = coords[0].y();
      int xe = coords[1].x();
      int ye = coords[1].y();
      coo[0] = Coordinate(xs,ys);
      coo[1] = Coordinate(xe,ys);
      coo[2] = Coordinate(xe,ye);
      coo[3] = Coordinate(xs,ye);
      coo[4] = Coordinate(xs,ys);
      PrimitivePolygon::SetCoordinates(coo);
    } else if (coords.size()==4) {
      std::vector<Coordinate> coo = coords;
      coo.push_back(coords[0]);
      PrimitivePolygon::SetCoordinates(coo);
    } else {
      PrimitivePolygon::SetCoordinates(coords);
    }
  }
  
}
