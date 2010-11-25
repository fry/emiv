#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/PrimitiveLine.hh>

namespace {
  struct line_data {
    float x_min, y_min, x_max, y_max, m;
  };
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
  
  std::vector<PrimitiveLine> PrimitivePolygon::BuildLines() {
    std::vector<line_data> edge_table;
    edge_table.reserve(coordinates_.size());

    // Draw the line strip from start to the last point
    for (int i = 0; i < coordinates_.size() - 1; i++) {
      edge_table.push_back(PrimitiveLine(GetColor(), coordinates_[i], coordinates_[i + 1]));
    }
    
    // Draw another line from end to start
    edge_table.push_back(PrimitiveLine(GetColor(), coordinates_[coordinates_.size() - 1], coordinates_[0]));
  }
  
  void PrimitivePolygon::Draw(ImageBase *im) {
    // Draw the line strip from start to the last point
    for (int i = 0; i < coordinates_.size() - 1; i++) {
      PrimitiveLine line(GetColor(), coordinates_[i], coordinates_[i + 1]);
      line.Draw(im);
    }
    
    // Draw another line from end to start
    PrimitiveLine line(GetColor(), coordinates_[coordinates_.size() - 1], coordinates_[0]);
    line.Draw(im);
  }
  
  void PrimitivePolygon::ScanlineFill() {
    // y = mx + b
    //  (y - b) / m = x
    std::vector<line_data> edge_table;
    edge_table.reserve(coordinates_.size() / 2 + 1);
    
    for (int i = 0; i < coordinates_.size() - 1; i++) {
      PrimitiveLine line(GetColor(), coordinates_[i], coordinates_[i + 1]);
      line.Draw(im);
    }
  }
}
