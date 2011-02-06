#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitivePolygon.hh>

#include <cassert>
#include <map>

using namespace Graphics2D;

int main(int argc, char** argv) {
  Image img;
  img.LoadPPM(argv[1]);
  img.SetColorModel(ImageBase::cm_Grey);

  StructureTensor t;
  t.SetFromImage(img);
  
  // detect corners
  std::vector<Coordinate> corners;
  t.HarrisCornerDetector(corners);

  // detect lines
  std::vector<PrimitiveLine> lines;
  HoughTransform hough;
  hough.StandardHoughTransform(img, 2, lines);
  
  // associate lines <-> corners
  typedef std::map<Coordinate*, std::vector<PrimitiveLine*> > pl_type;
  typedef std::map<PrimitiveLine*, std::vector<Coordinate*> > lp_type;
  pl_type point_line;
  lp_type line_point;
  for (std::vector<PrimitiveLine>::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
    iter->SetColor(Color::red());
    iter->Draw(&img);
    
    // compare corner <-> line distance for every corner
    std::cout << "line" << std::endl;
    for (std::vector<Coordinate>::iterator iter2 = corners.begin(); iter2 != corners.end(); ++iter2) {
      float distance = iter->Distance(*iter2);
      // associate corner <-> line if distance is low enough
      if (std::abs(distance) < 10) {
        point_line[&*iter2].push_back(&*iter);
        line_point[&*iter].push_back(&*iter2);
        std::cout << " dist " << distance  << ": " << iter2->fx() << "," << iter2->fy() << std::endl;
      }
    }
  }
  
  // iterate through all edges, calculate intersections, and print distances
  std::vector<Coordinate> points;
  for (std::vector<Coordinate>::iterator iter = corners.begin(); iter != corners.end(); ++iter) {
    std::vector<PrimitiveLine*>& intersect_lines(point_line[&*iter]);
    
    assert(intersect_lines.size() == 2);
    
    const PrimitiveLine& line1 = *intersect_lines[0];
    const PrimitiveLine& line2 = *intersect_lines[1];
    
    // remember intersection point for polygon
    Coordinate cut;
    line1.Intersection(line2, cut);
    
    std::cout << "intersection distance " << cut.Dist(*iter) << ": " << cut.fx() << "," << cut.fy() << std::endl;
  }
  
  // draw lines connection the edges
  for (lp_type::iterator iter = line_point.begin(); iter != line_point.end(); ++iter) {
    std::vector<Coordinate> line_points;
    PrimitiveLine line;
    line_points.push_back(*iter->second[0]);
    line_points.push_back(*iter->second[1]);
    line.SetCoordinates(line_points);
    line.SetColor(Color::green());
    line.Draw(&img);
  }
  
  /* Attempt to construct the polygon by starting at a random point (in this
   * case, the first point) and following the lines in a circle until we reach
   * the first point again. Because the order of the points in lines and the lines
   * can be random, we have to ensure we're not picking the same point or line
   * twice */

  // pick a starting point
  Coordinate* first(line_point.begin()->second[0]);
  Coordinate* current_point(first);
  // remember the current line
  PrimitiveLine* current_line(line_point.begin()->first);
  do {
    points.push_back(*current_point);
    
    const std::vector<PrimitiveLine*>& current_lines(point_line[current_point]);
    // pick the next line going through this point that is not the current line
    current_line = current_lines[0] != current_line ? current_lines[0] : current_lines[1];
    
    // pick the next point as the point in the line that is not the current point...
    const std::vector<Coordinate*>& current_points(line_point[current_line]);
    current_point = current_points[0] != current_point ? current_points[0] : current_points[1];
  } while (current_point != first);
  
  // draw the polygon in its own picture
  Image polyimg;
  polyimg.Init(img.GetWidth(), img.GetHeight());
  polyimg.FillZero();
  
  PrimitivePolygon pol;
  // a closed polygon
  points.push_back(*points.begin());

  pol.SetCoordinates(points);
  pol.SetColor(Color::red());
  pol.Draw(&polyimg);
   
  img.SavePPM("detectlines.ppm");
  std::cout << "saved detectlines.ppm" << std::endl;
  polyimg.SavePPM("polygon.ppm");
  std::cout << "saved polygon.ppm" << std::endl;
  
  return 0;
}
