/*
 * A Point Primitive
 * @author fkellner, 10/10
 */

#ifndef PRIMITIVEPOINT_HH_
#define PRIMITIVEPOINT_HH_

#include <Graphics2D/PrimitiveBase.hh>

namespace Graphics2D {

  class PrimitivePoint : public PrimitiveBase {
    public:
      PrimitivePoint();
      virtual ~PrimitivePoint();
      
      void SetCoordinate(const Coordinate &coo);
      
      void Draw(ImageBase *im);
      
  };

}

#endif /* PRIMITIVEPOINT_HH_ */
