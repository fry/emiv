#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <cmath>

namespace Graphics2D {
  class Coordinate {
  public:
    Coordinate(float x, float y);
    
    inline int GetX() const {
      return static_cast<int>(rint(x_));
    }

    inline int GetY() const {
      return static_cast<int>(rint(y_));
    }

    inline void SetX(float x) {
      x_ = x;
    }

    inline void SetY(float y) {
      y_ = y;
    }
  protected:
    float x_, y_;
  };
}

#endif
