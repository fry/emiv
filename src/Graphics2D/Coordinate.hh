/*
 * 2D Coordinate
 * @author fkellner, 10/10
 */

#ifndef COORDINATE_HH_
#define COORDINATE_HH_
#include <cmath>

namespace Graphics2D {

  class Coordinate {
    public:
      // coordinate defaults to (0,0)
      Coordinate(float x=0, float y=0) : x_(x), y_(y) {}
      virtual ~Coordinate() {}
      
      // change coordinate
      inline void Set(float x, float y) { x_ = x; y_ = y; }
      
      // get rounded coordinates
      inline int x() const {return (int)rint(x_);} 
      inline int y() const {return (int)rint(y_);}
      
      inline float fx() const {return x_;} 
      inline float fy() const {return y_;}
      
      // euclidean distance to other coordinate
      double Dist(const Coordinate &other) const;
      
      // operators for adding/substracting another coordinate
      Coordinate operator+(const Coordinate &other);
      Coordinate operator-(const Coordinate &other);
      // multiplication with scalar
      Coordinate operator*(float scalar);
      float operator*(const Coordinate& other);
      
      // rotate coordinate around origin with angle in radians
      void Rotate(const Coordinate &origin, float angle);

      float Length();
    protected:
      float x_,y_;
  };

}

#endif /* COORDINATE_HH_ */
