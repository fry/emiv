/*
 * A Line
 * @author fkellner, 10/10
 */

#ifndef PRIMITIVELINE_HH_
#define PRIMITIVELINE_HH_

#include <Graphics2D/PrimitiveBase.hh>

namespace Graphics2D {

  class PrimitiveLine : public PrimitiveBase {
    public:
      PrimitiveLine();

      virtual ~PrimitiveLine();
      
      virtual void SetCoordinates(const std::vector<Coordinate> &coords);

      virtual void Draw(ImageBase *im);
      
      
      // following functions are used by scanline algorithm
      
      /** 
       * make edge active if y==ymin
       * @param y current scanline
       * @return true if y==ymin and line is not horizontal and was not active before
       */ 
      bool MakeActive(int y);

      /** 
       * make edge inactive if y==ymax
       * @param y current scanline
       * @return true if y==ymax and edge was active
       */ 
      bool MakeInactive(int y);
      
      /**
       * intersection with scanline is updated according to line slope.
       * @return true if line is active
       */
      bool UpdateIntersection();
      
      // sort function for comparing ymin
      static inline bool CompareYmin(const PrimitiveLine &pl1, const PrimitiveLine &pl2) {
        return pl1.ymin_ < pl2.ymin_;
      }
      
      // sort function for comparing intersections on a scanline
      static inline bool CompareIntersection(const PrimitiveLine &pl1, const PrimitiveLine &pl2) {
        return pl1.intersection_ < pl2.intersection_;
      }
      
      inline int ymin() {
        return ymin_;
      }
      
      inline int ymax() {
        return ymax_;
      }
      
      inline int intersection() {
        return rint(intersection_);
      }
      
      float Distance(const Coordinate& coord);
      
      void Intersection(const PrimitiveLine& other, Coordinate& out_intersect) const;
    protected:
      // if line is y=ax+b, dxdy is 1/a. Set by SetCoordinates
      float dxdy_;
      // current intersection with scanline. Set by UpdateIntersection.
      float intersection_;
      // minimal and maximal y coords. Set by SetCoordinates
      int ymin_, ymax_; 
      // x coordinate at ymin. Set by SetCoordinates
      int xymin_;
      // line is active as long as current scanline intersects it. Set by MakeActive/MakeInactive
      bool active_;
      // horizontal and vertical line indicators. Set by SetCoordinates
      bool horiz_, vert_;
      // true after coordinates are set. Set by SetCoordinates
      bool inited_;
  };

}

#endif /* PRIMITIVELine_HH_ */
