#include <Graphics2D/PrimitiveLine.hh>

namespace Graphics2D {
  PrimitiveLine::PrimitiveLine(const Color& color, const Coordinate& start, const Coordinate& end) : PrimitiveBase(color) {
    coordinates_.push_back(start);
    coordinates_.push_back(end);
    name_="Line";
  }
  
  
  void PrimitiveLine::Draw(ImageBase* im) {
    Coordinate start = GetCoordinates()[0];
    Coordinate end = GetCoordinates()[1];

    int x0 = start.GetX();
    int y0 = start.GetY();
    int x1 = end.GetX();
    int y1 = end.GetY();
    
    // Bresenham can only draw lines going up a max of 45 degrees
    // If the line is steeper than 45 degrees, mirror it on the diagonal axis
    bool mirrored = std::abs(x1 - x0) < std::abs(y1 - y0);
    if (mirrored) {
      std::swap(x0, y0);
      std::swap(x1, y1);
    }

    // Swap start and end point if the former is further right than the latter
    if (x1 < x0) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
    
    // dx is already going to be positive due to the previous swapping
    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);

    // Setup loop variables
    // If the line goes downwards, we have to decrease y instead of increase
    int stepy = y0 < y1 ? 1 : -1;
    int y = y0;
    int e = 2*dx-dy;
    
    const Color& color = GetColor();
    
  	const int width = im->GetWidth();
  	const int height = im->GetHeight();
	
	// variables to store the coordinates to be drawn later
    int drawx, drawy=0;
    
    for (int x = x0; x < x1; x++) {
      // Decide whether to move up depending on the decision value
      if (e > 0) {
        y += stepy;
        e -= 2*dx;
      }
      
      e += 2*dy;
        
      // Figure out where to draw the line, if we mirrored it, we need
      // to swap x/y coordinates before drawing
      drawx = x;
      drawy = y;
      
      if (mirrored) {
        drawx = y;
        drawy = x;
      }
      
      // Ensure that the point is inside the window
	    if (drawx >= 0 && drawx < width && drawy >= 0 && drawy < height) {
    		im->SetPixel(drawx, drawy, 0, color.GetRed());
    		im->SetPixel(drawx, drawy, 1, color.GetGreen());
    		im->SetPixel(drawx, drawy, 2, color.GetBlue());
      }
    }
  }
}
