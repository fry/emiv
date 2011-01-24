#ifndef PRIMITIVEBOX_HH_
#define PRIMITIVEBOX_HH_

#include <Graphics2D/PrimitivePolygon.hh> 

namespace Graphics2D {

  class PrimitiveBox : public PrimitivePolygon {
    public:
      PrimitiveBox();
      virtual ~PrimitiveBox();
      
      void SetCoordinates(const std::vector<Coordinate> &coords);
  };

}

#endif /* PRIMITIVEBOX_HH_ */
