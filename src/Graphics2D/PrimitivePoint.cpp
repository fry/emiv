#include <Graphics2D/PrimitivePoint.hh>

namespace Graphics2D {
  PrimitivePoint::PrimitivePoint(const Color& color, const Coordinate& coordinate): PrimitiveBase(color) {
    SetCoordinate(coordinate);
    name_="Point";
  }
  
  void PrimitivePoint::Draw(ImageBase* im) {
    const Coordinate& coord = GetCoordinate();
    const Color& color = GetColor();
    im->SetPixel(coord.GetX(), coord.GetY(), 0, color.GetRed());
    im->SetPixel(coord.GetX(), coord.GetY(), 1, color.GetGreen());
    im->SetPixel(coord.GetX(), coord.GetY(), 2, color.GetBlue());
  }
}
