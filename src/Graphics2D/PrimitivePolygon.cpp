#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/PrimitiveLine.hh>
#include <Graphics2D/PrimitivePoint.hh>

#include <Graphics2DBase/Coordinate.hh>

#include <map>
#include <list>

namespace {
  struct line_data {
    int y_min, y_max;
    float x_min, inv_m;
    bool vertical;
    
    line_data(const Graphics2D::Coordinate& start, const Graphics2D::Coordinate& end) {
      const int sx = start.GetX();
      const int sy = start.GetY();
      const int ex = end.GetX();
      const int ey = end.GetY();
      int x_max;

      vertical = (ex - sx) == 0;

      // Find smallest y position
      if (sy < ey) {
        y_min = sy;
        y_max = ey;
        x_min = sx;
        x_max = ex;
      } else {
        y_min = ey;
        y_max = sy;
        x_min = ex;
        x_max = sx;
      }

      if (!vertical) {
        // the x_max - x_min difference decides the sign of the slope
        inv_m =  (x_max - x_min) / static_cast<float>(y_max - y_min);
      }
    }
  };
  
  bool sort_x(const line_data& a, const line_data& b) {
    return a.x_min < b.x_min;
  }
}

namespace Graphics2D {
  PrimitivePolygon::PrimitivePolygon(const Color& color): PrimitiveBase(color) {
    name_ = "Polygon";
  }
  
  PrimitivePolygon::PrimitivePolygon(const Color& color, const std::vector<Coordinate> points): PrimitiveBase(color) {
    SetCoordinates(points);
  }
  
  void PrimitivePolygon::SetCoordinates(const std::vector<Coordinate>& coordinates) {
    if (coordinates.size() < 2)
      throw std::runtime_error("A Polygon requires at least two points.");
    
    coordinates_ = coordinates;
  }
  
  void PrimitivePolygon::Draw(ImageBase *im) {
    // Draw the line strip from start to the last point
    /*for (int i = 0; i < coordinates_.size() - 1; i++) {
      PrimitiveLine line(GetColor(), coordinates_[i], coordinates_[i + 1]);
      line.Draw(im);
    }
    
    // Draw another line from end to start
    PrimitiveLine line(GetColor(), coordinates_[coordinates_.size() - 1], coordinates_[0]);
    line.Draw(im);*/
    
    // Fill polygon
    ScanlineFill(im);
  }
  
  void PrimitivePolygon::ScanlineFill(ImageBase* im) {
    typedef std::multimap<int, line_data> table_type;
    typedef std::pair<int, line_data> pair_type;
    table_type edge_table;

    // Draw the line strip from start to the last point
    for (int i = 0; i < coordinates_.size() - 1; i++) {
      line_data line(coordinates_[i], coordinates_[i + 1]);
      edge_table.insert(pair_type(line.y_min, line));
    }
    
    // Draw another line from end to start
    line_data line(coordinates_[coordinates_.size() - 1], coordinates_[0]);
    edge_table.insert(pair_type(line.y_min, line));
    
    // Start at the smallest line
    int y = edge_table.begin()->first;
    
    // active edge table
    std::list<line_data> active_edge_table;
    std::list<line_data>::iterator aet_iter, aet_end;
    aet_end = active_edge_table.end();
    
    // Set up edge_table iterators
    table_type::iterator iter, end;
    std::pair<table_type::iterator, table_type::iterator> results;
    end = edge_table.end();
    
    // the point to draw for lines
    PrimitivePoint point(GetColor());
    while (!edge_table.empty() || !active_edge_table.empty()) {
      // add lines starting here
      results = edge_table.equal_range(y);
      for (iter = results.first; iter != results.second; ++iter) {
        active_edge_table.push_back(iter->second);
      }
      
      // remove them from the edge table
      edge_table.erase(results.first, results.second);
      
      // sort active edge table
      active_edge_table.sort(::sort_x);
      
      // for each active line, go through the sorted intersections, counting parity
      int parity = 0;
      int last_x = 0;
      for (aet_iter = active_edge_table.begin(); aet_iter != aet_end; ) {
        // Round down intersection point
        int intersect_x = (int)aet_iter->x_min;
        
        // Draw a horizontal line once we hit a second intersection
        if (parity % 2 != 0) {
          for (int x = last_x; x <= intersect_x; x++) {
            point.SetCoordinate(Coordinate(x, y));
            point.Draw(im);
          }
        }
        
        // Don't count intersection if it's on y_max (to avoid problems with two meeting lines)
        if (y != aet_iter->y_max)
          parity ++;

        last_x = intersect_x;
        
        // remove element if it is leaving the interesting area, otherwise update x
        if (aet_iter->y_max == y) {
          aet_iter = active_edge_table.erase(aet_iter);
        } else {
          // update line position if the line is not vertical
          if (!aet_iter->vertical) {
            aet_iter->x_min += aet_iter->inv_m;
          }
        
          // increment iterator if we didn't remove an element
          ++aet_iter;
        }
      }
      
      // continue with next line
      y ++;
    }
  }
}
