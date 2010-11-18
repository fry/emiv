#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/PrimitiveLine.hh>

namespace Graphics2D {
  PrimitivePolygon::PrimitivePolygon(const Color& color): PrimitiveBase(color) {
  name_="Polygon";
  }
  
  PrimitivePolygon::PrimitivePolygon(const Color& color, const std::vector<Coordinate> points): PrimitiveBase(color) {
    SetCoordinates(points);
  }
  
  void PrimitivePolygon::SetCoordinates(const std::vector<Coordinate>& coordinates) {
    if (coordinates.size() < 3)
      throw std::runtime_error("A Polygon requires at least three points.");
    
    coordinates_ = coordinates;
  }
  
  void PrimitivePolygon::Draw(ImageBase *im) {
    
    for (int i = 0; i < coordinates_.size() - 1; i++) {
      PrimitiveLine line(GetColor(), coordinates_[i], coordinates_[i + 1]);
      line.Draw(im);
    }
    
    PrimitiveLine line(GetColor(), coordinates_[coordinates_.size() - 1], coordinates_[0]);
    line.Draw(im);
  }
}
