#ifndef PRIMITIVESTAR_HH
#define PRIMITIVESTAR_HH

#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2DBase/ImageBase.hh>
#include <vector>

namespace Graphics2D {
  class PrimitiveStar: public PrimitivePolygon {
  public:
    PrimitiveStar(const Color& color, const Coordinate& position, float radius, int edge_count = 8);
    
    //virtual void SetCoordinates(const std::vector<Coordinate>& coordinates);
  };
}

#endif
