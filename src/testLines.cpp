#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>

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
  
  for (std::vector<PrimitiveLine>::iterator iter = lines.begin(); iter != lines.end(); ++iter) {
    iter->SetColor(Color::red());
    iter->Draw(&img);
    
    std::cout << "line" << std::endl;
    for (std::vector<Coordinate>::iterator iter2 = corners.begin(); iter2 != corners.end(); ++iter2) {
      std::cout << iter2->fx() << "," << iter2->fy() << ": " << std::endl;
      std::cout << " dist " << iter->Distance(*iter2) << std::endl;
    }
  }
  
  img.SavePPM("detectlines.ppm");
  
  return 0;
}
