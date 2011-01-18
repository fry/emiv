#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/ColorConversion.hh>

using namespace Graphics2D;

void test_image(const std::string name) {
  std::cout << name.substr(name.find_last_of("/") + 1, std::string::npos) << ":" << std::endl;
  Image image;
  image.LoadPPM(name);
  image.SetColorModel(ImageBase::cm_Grey);
  
  const int label = 255;
  Segmentation seg(image);
  Coordinate center; 
  int area;
  seg.GetCenterAndArea(label, center, area);
  std::cout << "  Object has its center at: " << center.GetX() << "," << center.GetY() << " with area: " << area << " pixel." << std::endl;
  
  Coordinate topleft(seg.GetLabelTopLeft(label));
  std::vector<int> fmc;
  seg.GetFreemanCode(label, topleft, fmc);

  const float circumference = seg.GetCircumference(fmc);
  const float roundness = seg.GetRoundness(area, circumference);
  std::cout << "  Object has roundness of " << roundness << std::endl;
  
  if (rint(roundness) >= 16) {
    std::cout << "  Object is a rectangle" << std::endl;
  } else {
    std::cout << "  Object is a circle" << std::endl;
  }
}

int main(int argc, char** argv) {
  test_image("../serie9-aufgabe2-testbilder/circle1.ppm");
  test_image("../serie9-aufgabe2-testbilder/circle2.ppm");
  test_image("../serie9-aufgabe2-testbilder/circle3.ppm");
  test_image("../serie9-aufgabe2-testbilder/circle4.ppm");
  
  test_image("../serie9-aufgabe2-testbilder/rectangle1.ppm");
  test_image("../serie9-aufgabe2-testbilder/rectangle2.ppm");
  test_image("../serie9-aufgabe2-testbilder/rectangle3.ppm");
  test_image("../serie9-aufgabe2-testbilder/rectangle4.ppm");
  
  test_image("../serie9-aufgabe2-testbilder/tree1.ppm");
  test_image("../serie9-aufgabe2-testbilder/tree2.ppm");
  test_image("../serie9-aufgabe2-testbilder/tree3.ppm");
  test_image("../serie9-aufgabe2-testbilder/tree4.ppm");
  return 0;
}
