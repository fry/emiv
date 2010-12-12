#include <Graphics2DBase/Coordinate.hh>
#include <cmath>

namespace Graphics2D {
  Coordinate::Coordinate() {
    SetX(0);
    SetY(0);  
  }
  
  Coordinate::Coordinate(float x, float y) {
    SetX(x);
    SetY(y);
  }
  
  void Coordinate::Rotate(const Coordinate& origin, float angle) {
    // Translate to origin
    *this -= origin;
    
    // Apply rotation
    const float c = std::cos(angle);
    const float s = std::sin(angle);
    
    const float new_x = x_ * c - y_ * s;
    const float new_y = y_ * c + x_ * s;
    
    SetX(new_x);
    SetY(new_y);
    
    // Translate back to old position
    *this += origin;
  }
}
