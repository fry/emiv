#ifndef PRIMITIVEPOLYGON_HH
#define PRIMITIVEPOLYGON_HH

#include <Graphics2DBase/Coordinate.hh>
#include <Graphics2DBase/PrimitiveBase.hh>
#include <Graphics2DBase/ImageBase.hh>
#include <Graphics2DBase/Color.hh>
#include <vector>

namespace Graphics2D {
  class PrimitivePolygon: public PrimitiveBase {
  public:
    PrimitivePolygon(const Color& color);
    PrimitivePolygon(const Color& color, const std::vector<Coordinate> points);
    
    virtual void SetCoordinates(const std::vector<Coordinate>& coordinates);
    
    void Draw(ImageBase* im);
  protected:
    
    std::vector<PrimitiveLine> BuildLines(); 
  };
}

#endif
