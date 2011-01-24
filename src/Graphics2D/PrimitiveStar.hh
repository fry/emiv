#ifndef PRIMITIVESTAR_HH_
#define PRIMITIVESTAR_HH_

#include <Graphics2D/PrimitivePolygon.hh> 

namespace Graphics2D {

  class PrimitiveStar : public PrimitivePolygon {
    public:
      PrimitiveStar(int numCones=8);
      virtual ~PrimitiveStar();
      
      virtual void SetCoordinates(const std::vector<Coordinate> &coords);
      
    protected:
      int numCones_;
  };

}

#endif /* PRIMITIVESTAR_HH_ */
