#include <Graphics2D/PrimitivePolygon.hh>
#include <vector>
#include <algorithm>
#include <Graphics2D/PrimitivePoint.hh>

namespace Graphics2D {

  PrimitivePolygon::PrimitivePolygon() {
    name_ = "PrimitivePolygon";
    closed_ = false;
    coords_.clear();
  }

  PrimitivePolygon::~PrimitivePolygon() {
    // TODO Auto-generated destructor stub
  }
  
  void PrimitivePolygon::SetColor(const Color &color) {
    color_ = color;
    for (unsigned int i=0;i<lines_.size();i++) {
      lines_[i].SetColor(color_);
    }
  }

  void PrimitivePolygon::SetCoordinates(const std::vector<Coordinate> &coords) {
    if (coords.size() < 2) return;
    coords_ = coords;
    int csize = (int)coords_.size();
    if (csize >= 3) {
      if (coords_[0].Dist(coords_[csize-1])<20) {
        closed_ = true;
        coords_[csize-1] = coords[0];
      } else {
        closed_ = false;
      }
    }
    lines_.resize(csize-1);
    std::vector<Coordinate> coo(2);
    for (int i=0;i<csize-1;i++) {
      coo[0] = coords_[i];
      coo[1] = coords_[i+1];
      lines_[i].SetCoordinates(coo);
      lines_[i].SetColor(color_);
    }
    if (closed_) {
      coords_.pop_back();
    }
  }
  
  void PrimitivePolygon::Rotate(float angle) {
    if (!closed_) return;
    // rotate using all coordinates
    PrimitiveBase::Rotate(angle);
    // update all lines
    std::vector<Coordinate> coo(2);
    int csize = (int)coords_.size();
    for (int i=0;i<csize;i++) {
      coo[0] = coords_[i];
      coo[1] = coords_[(i+1)%csize];
      lines_[i].SetCoordinates(coo);
    }
  }
  
  void PrimitivePolygon::Draw(ImageBase *im) {
    if (coords_.size() < 2) return;
    if (closed_) {
      // all closed polygons are flood filled with scanline algorithm
      ScanlineFill_(im);
    } else {
      for (unsigned int i=0;i<lines_.size();i++) {
        lines_[i].Draw(im);
      }
    }
  }

  
  void PrimitivePolygon::ScanlineFill_(ImageBase *im) {
    
    int w = (int)im->GetWidth();
    int h = (int)im->GetHeight();
    unsigned char *data = im->GetData();

    // our lines_ vector is the edgetable, this local copy is just to make 
    // the algorithm more understandable
    std::vector<PrimitiveLine> edgeTable;
    edgeTable = lines_;
    // currently active edges
    std::vector<PrimitiveLine> activeTable;
    activeTable.clear();
    
    // sort edges by miny
    std::sort(edgeTable.begin(), edgeTable.end(), PrimitiveLine::CompareYmin);
    
    // we start at lowest y
    int y = edgeTable[0].ymin();
    
    // iterate y
    do {

      // edges become active if their ymin == y
      for (unsigned int i=0;i<edgeTable.size();i++) {
        if (edgeTable[i].MakeActive(y)) {
          activeTable.push_back(edgeTable[i]);
        }
      }
      
      // clear inactive edges from active edge list
      
      // iterate through active edge list
      std::vector<PrimitiveLine>::iterator iter = activeTable.begin();
      
      while (iter != activeTable.end()) {
        if (iter->MakeInactive(y)) {
          // if y == ymax, an edge is marked inactive
          // erase(iter) returns a valid iterator to element after removed one 
          iter = activeTable.erase( iter );
        } else {
          // keep active edges
          iter++;
        }
      }
      
      // only try drawing if inside image
      if (y>=0 && y<h) {
      
        // sort intersections of active edges
        std::sort(activeTable.begin(), activeTable.end(), PrimitiveLine::CompareIntersection);
        
        // we might draw something if there is two intersections at least
        if (activeTable.size()>=2 && activeTable.size()%2==0) {
          // draw spans
          for (unsigned int idx = 0; idx < activeTable.size()-1; idx+=2) {
            int xmin = std::max(0,activeTable[idx].intersection());
            int xmax = std::min(w-1,activeTable[idx+1].intersection());
            for (int x=xmin;x<=xmax;x++) {
              data[(y*w+x)*3+0] = color_.r();
              data[(y*w+x)*3+1] = color_.g();
              data[(y*w+x)*3+2] = color_.b();
            }
          }
        }
      }
      
      // increase y
      y++;
      
      // on each increasing y, the intersection is updated according to slope
      for (unsigned int j=0;j<activeTable.size();j++) {
        activeTable[j].UpdateIntersection();
      }

    } while (!edgeTable.empty() && !activeTable.empty());
  }
}
