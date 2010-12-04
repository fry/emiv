#ifndef PRIMITIVELINE_HH
#define PRIMITIVELINE_HH

#include <Graphics2DBase/PrimitiveBase.hh>
#include <Graphics2DBase/ImageBase.hh>

namespace Graphics2D {
  class PrimitiveLine: public PrimitiveBase {
  public:
    PrimitiveLine(const Color& color, const Coordinate& start, const Coordinate& end);
    void Draw(ImageBase* im);
  protected:
    float m;
  };
}

#endif
