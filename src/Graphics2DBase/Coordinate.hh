#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <cmath>

namespace Graphics2D {
  class Coordinate {
  public:
    Coordinate();
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
    
    inline Coordinate operator+(const Coordinate& other) {
      return Coordinate(x_ + other.x_, y_ + other.y_);
    }
    
    inline Coordinate operator-(const Coordinate& other) {
      return Coordinate(x_ - other.x_, y_ - other.y_);
    }
    
    inline Coordinate operator*(float scalar) {
      return Coordinate(x_ * scalar, y_ * scalar);
    }
    
    inline Coordinate& operator+=(const Coordinate& other) {
      SetX(x_ + other.x_);
      SetY(y_ + other.y_);
      return *this;
    }
    
    inline Coordinate& operator-=(const Coordinate& other) {
      SetX(x_ - other.x_);
      SetY(y_ - other.y_);
      return *this;
    }
    
    inline Coordinate& operator*=(float scalar) {
      SetX(x_ * scalar);
      SetY(y_ * scalar);
      return *this;
    }
    
    void Rotate(const Coordinate& origin, float angle);
  protected:
    float x_, y_;
  };
}

#endif
