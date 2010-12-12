#include <Graphics2D/PrimitiveBox.hh>

namespace Graphics2D {
  PrimitiveBox::PrimitiveBox(const Color& color, const Coordinate& a, const Coordinate& b): PrimitivePolygon(color) {
    std::vector<Coordinate> points;
    points.reserve(2);
    points.push_back(a);
    points.push_back(b);
    SetCoordinates(points);
    name_ = "Box";
  }
  
  PrimitiveBox::PrimitiveBox(const Color& color, const std::vector<Coordinate>& points): PrimitivePolygon(color) {
    SetCoordinates(points);
    name_="Box";
  }
  
  void PrimitiveBox::SetCoordinates(const std::vector<Coordinate>& coordinates) {
    if (coordinates.size() != 2)
      throw std::runtime_error("A box requires two points.");
    
    const Coordinate topleft = coordinates[0];
    const Coordinate bottomright = coordinates[1];

    coordinates_.clear();
    coordinates_.reserve(4);
    coordinates_.push_back(topleft);
    coordinates_.push_back(Coordinate(bottomright.GetX(), topleft.GetY()));
    coordinates_.push_back(bottomright);
    coordinates_.push_back(Coordinate(topleft.GetX(), bottomright.GetY()));
  }
}
