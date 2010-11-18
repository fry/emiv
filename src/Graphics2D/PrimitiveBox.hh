#ifndef PRIMITIVEBOX_HH
#define PRIMITIVEBOX_HH

#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2DBase/ImageBase.hh>
#include <vector>

namespace Graphics2D {
  class PrimitiveBox: public PrimitivePolygon {
  public:
    PrimitiveBox(const Color& color, const std::vector<Coordinate>& points);
    
    virtual void SetCoordinates(const std::vector<Coordinate>& coordinates);
  };
}

#endif
