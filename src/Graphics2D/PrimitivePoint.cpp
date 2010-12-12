#include <Graphics2D/PrimitivePoint.hh>

namespace Graphics2D {
  PrimitivePoint::PrimitivePoint(const Color& color): PrimitiveBase(color) {  
  }
  
  PrimitivePoint::PrimitivePoint(const Color& color, const Coordinate& coordinate): PrimitiveBase(color) {
    SetCoordinate(coordinate);
    name_ = "Point";
  }
  
  void PrimitivePoint::Draw(ImageBase* im) {
    const Coordinate& coord = GetCoordinate();
    const Color& color = GetColor();

    const int x = coord.GetX();
    const int y = coord.GetY();
    
    if (x < 0 || y < 0 || x >= im->GetWidth() || y >= im->GetHeight())
      return;

    im->SetPixel(x, y, 0, color.GetRed());
    im->SetPixel(x, y, 1, color.GetGreen());
    im->SetPixel(x, y, 2, color.GetBlue());
  }
}
