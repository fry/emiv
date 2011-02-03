#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>

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
  
  typedef std::multimap<Coordinate*, PrimitiveLine*> pl_type;
  typedef std::multimap<PrimitiveLine*, Coordinate*> lp_type;
  pl_type point_line;
  lp_type line_point;
  for (std::vector<PrimitiveLine>::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
    iter->SetColor(Color::red());
    iter->Draw(&img);
    
    std::cout << "line" << std::endl;
    for (std::vector<Coordinate>::iterator iter2 = corners.begin(); iter2 != corners.end(); ++iter2) {
      float distance = iter->Distance(*iter2);
      if (std::abs(distance) < 10) {
        point_line.insert(std::pair<Coordinate*, PrimitiveLine*>(&*iter2, &*iter));
        line_point.insert(std::pair<PrimitiveLine*, Coordinate*>(&*iter, &*iter2));
        std::cout << " dist " << distance  << std::endl;
      }
    }
  }
  
  for (std::vector<PrimitiveLine>::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
    
  }
  
  img.SavePPM("detectlines.ppm");
  
  return 0;
}
