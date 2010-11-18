#ifndef PRIMITIVEPOINT_HH
#define PRIMITIVEPOINT_HH

#include <Graphics2DBase/PrimitiveBase.hh>

namespace Graphics2D {
  class PrimitivePoint: public PrimitiveBase {
  public:
    PrimitivePoint(const Color& color, const Coordinate& coordinate);
    
    inline const Coordinate& GetCoordinate() const {
      return coordinates_[0];
    }
    
    inline void SetCoordinate(const Coordinate& coordinate) {
      coordinates_.clear();
      coordinates_.push_back(coordinate);
    }
    
    void Draw(ImageBase* im);
  };
}

#endif
