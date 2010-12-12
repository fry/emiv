#include <Graphics2D/PrimitiveStar.hh>

namespace Graphics2D {
  PrimitiveStar::PrimitiveStar(const Color& color, const Coordinate& position, float radius, int edge_count): PrimitivePolygon(color) {
    const float step = 2*M_PI / edge_count / 2.0;
    
    std::vector<Coordinate> points;
    points.reserve(edge_count * 2);
    const Coordinate origin;
    float rotation = 0;
    for (int i = 0; i < edge_count; i++) {
      // Outer point
      Coordinate point(0, -radius * 2);
      point.Rotate(origin, rotation);
      point += position;
      points.push_back(point);
      rotation += step;
      
      // Inner point
      point = Coordinate(0, -radius);
      point.Rotate(origin, rotation);
      point += position;
      points.push_back(point);
      rotation += step;
    }
    
    SetCoordinates(points);
  }
}