#include <Graphics2D/Coordinate.hh>
#include <iostream>

namespace Graphics2D {

  double Coordinate::Dist(const Coordinate &other) const {
    return sqrt((x_-other.x_)*(x_-other.x_)+(y_-other.y_)*(y_-other.y_));
  }

  Coordinate Coordinate::operator+(const Coordinate &other) {
    return Coordinate(x_+other.x_, y_+other.y_);
  }
  Coordinate Coordinate::operator-(const Coordinate &other) {
    return Coordinate(x_-other.x_, y_-other.y_);
  }
  Coordinate Coordinate::operator*(float scalar) {
    return Coordinate(x_*scalar, y_*scalar);
  }
  
  void Coordinate::Rotate(const Coordinate &origin, float angle) {
    Coordinate shifted = *this-origin;
    x_ = ((cos(angle)*shifted.x_ - sin(angle)*shifted.y_)) + origin.x();
    y_ = ((sin(angle)*shifted.x_ + cos(angle)*shifted.y_)) + origin.y();
  }

  float Coordinate::operator*(const Coordinate& other) {
    return x_ * other.x_ + y_ * other.y_;
  }
  
  float Coordinate::Length() {
    return std::sqrt(x_*x_ + y_*y_);
  }
}
