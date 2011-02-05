#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitivePolygon.hh>

#include <map>

using namespace Graphics2D;

int main(int argc, char** argv) {
  Image img;
  img.LoadPPM(argv[1]);
  img.SetColorModel(ImageBase::cm_Grey);

  StructureTensor t;
  t.SetFromImage(img);
  
  std::vector<Coordinate> corners;
  t.HarrisCornerDetector(corners);

  std::vector<PrimitiveLine> lines;
  HoughTransform hough;
  hough.StandardHoughTransform(img, 2, lines);
  
  typedef std::map<Coordinate*, std::vector<PrimitiveLine*> > pl_type;
  typedef std::map<PrimitiveLine*, std::vector<Coordinate*> > lp_type;
  pl_type point_line;
  lp_type line_point;
  for (std::vector<PrimitiveLine>::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
    iter->SetColor(Color::red());
    iter->Draw(&img);
    
    std::cout << "line" << std::endl;
    for (std::vector<Coordinate>::iterator iter2 = corners.begin(); iter2 != corners.end(); ++iter2) {
      float distance = iter->Distance(*iter2);
      if (std::abs(distance) < 10) {
        point_line[&*iter2].push_back(&*iter);
        line_point[&*iter].push_back(&*iter2);
        std::cout << " dist " << distance  << ": " << iter2->fx() << "," << iter2->fy() << std::endl;
      }
    }
  }
  
  // iterate through all edges, calculate intersections, and print distances
  PrimitivePolygon pol;
  std::vector<Coordinate> points;
  for (std::vector<Coordinate>::iterator iter = corners.begin(); iter != corners.end(); ++iter) {
    std::vector<PrimitiveLine*>& intersect_lines(point_line[&*iter]);
    
    assert(intersect_lines.size() == 2);
    
    const PrimitiveLine& line1 = *intersect_lines[0];
    const PrimitiveLine& line2 = *intersect_lines[1];
    
    Coordinate cut;
    line1.Intersection(line2, cut);
    points.push_back(cut);
    
    std::cout << "intersection distance " << cut.Dist(*iter) << ": " << cut.fx() << "," << cut.fy() << std::endl;
  }
  
  for (lp_type::iterator iter = line_point.begin(); iter != line_point.end(); ++iter) {
    std::vector<Coordinate> line_points;
    PrimitiveLine line;
    line_points.push_back(*iter->second[0]);
    line_points.push_back(*iter->second[1]);
    line.SetCoordinates(line_points);
    line.SetColor(Color::green());
    line.Draw(&img);
  }
  
  img.SavePPM("detectlines.ppm");
  
  return 0;
}
