/*
 * A polygon is drawn by drawing a closed loop of lines 
 */

#ifndef POLYGON_HH_
#define POLYGON_HH_

#include <Graphics2D/PrimitiveLine.hh>

namespace Graphics2D {

  class PrimitivePolygon : public PrimitiveBase {
    public:
      PrimitivePolygon();
      virtual ~PrimitivePolygon();
      
      virtual void SetColor(const Color &color);
      virtual void SetCoordinates(const std::vector<Coordinate> &coords);

      virtual void Draw(ImageBase *im);
      
      inline bool closed() { return closed_; }
      
      virtual void Rotate(float angle);

    protected:
      void ScanlineFill_(ImageBase *im);
      
      std::vector<PrimitiveLine> lines_;
      
      bool closed_;
      
  };

}

#endif /* POLYGON_HH_ */
