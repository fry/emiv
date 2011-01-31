
#include <cstdlib>
#include <vector>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitiveLine.hh>

namespace Graphics2D {

  PrimitiveLine::PrimitiveLine() {
    // line has start and end point
    coords_.resize(2);
    name_ = "Line";
    inited_ = false;
    active_ = false;
    horiz_ = false;
    vert_ = false;
    intersection_ = -1;
  }
  
  PrimitiveLine::~PrimitiveLine() {
    // TODO Auto-generated destructor stub
  }

  void PrimitiveLine::SetCoordinates(const std::vector<Coordinate> &coords) {
    PrimitiveBase::SetCoordinates(coords);
    
    horiz_ = false;
    vert_ = false;
    
    // determine limits
    if (coords_[0].y()<=coords_[1].y()) {
      ymax_ = coords_[1].y();
      ymin_ = coords_[0].y();
      xymin_ = coords_[0].x();
    } else {
      ymax_ = coords_[0].y();
      ymin_ = coords_[1].y();
      xymin_ = coords_[1].x();
    }
    
    // determine if horizonal or vertical
    if (coords_[0].x() == coords_[1].x()) {
      vert_ = true;
      dxdy_ = 0.0f;
    }
    if (coords_[0].y() == coords_[1].y()) {
      horiz_ = true;
    }
    
    // calculate slope
    if (!horiz_ && !vert_) {
      float x0=float(coords_[0].x());
      float y0=float(coords_[0].y());
      float x1=float(coords_[1].x());
      float y1=float(coords_[1].y());
      dxdy_ = (x1-x0) / (y1-y0);
    }
    // indicate that everything is set correctly
    inited_ = true;
  }

  void PrimitiveLine::Draw(ImageBase *im) {
    
    // bresenham line draw
    int x0,y0,x1,y1,tmp;
    
    bool steep = abs(coords_[1].y() - coords_[0].y()) > abs(coords_[1].x() - coords_[0].x());
    if (steep) {
      x0 = coords_[0].y();
      y0 = coords_[0].x();
      x1 = coords_[1].y();
      y1 = coords_[1].x();
    } else {
      x0 = coords_[0].x();
      y0 = coords_[0].y();
      x1 = coords_[1].x();
      y1 = coords_[1].y();
    }
    if (x0>x1) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = deltax / 2;
    int ystep;
    int y = y0;
    if (y0 < y1) {
      ystep = 1; 
    } else {
      ystep = -1;
    }
    
    // use point primitive to draw single points
    PrimitivePoint p;
    Coordinate coo;
    p.SetColor(color_);
    
    // all initialisation finished, start drawing finally
    for (int x=x0;x<=x1;x++) {
      if (steep) {
        coo.Set(y,x);
      } else {
        coo.Set(x,y);
      }
      p.SetCoordinate(coo);
      p.Draw(im);
      error = error - deltay;
      if (error < 0) {
        y = y + ystep;
        error = error + deltax;
      }
    }
  }
  
  bool PrimitiveLine::MakeActive(int y) {
    if (active_) {
      return false;
    }
    if (y==ymin_ && !horiz_) {
      active_ = true;
      intersection_ = xymin_;
      return true;
    } else {
      return false;
    }
  }
  
  bool PrimitiveLine::MakeInactive(int y) {
    if (!active_ || !inited_) return true;
    if (active_ && y==ymax_) {
      active_ = false;
      return true;
    }
    return false;
  }
  
  bool PrimitiveLine::UpdateIntersection() {
    if (!active_) return false;
    intersection_ += dxdy_;
    return true;
  }
  
  float PrimitiveLine::Distance(const Coordinate& coord) {
    Coordinate direction = coords_[1] - coords_[0];
    Coordinate normal(direction.fx(), -direction.fy());
    normal = normal * (1.0f / normal.Length());
    return normal * (coord - coords_[0]);
  }
}
