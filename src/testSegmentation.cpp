#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/ColorConversion.hh>

using namespace Graphics2D;

void print_area_center(Segmentation seg, const std::string& name, int label) {
  int area; Coordinate center;
  seg.GetCenterAndArea(label, center, area);
  std::cout << name << " is located at " << center.GetX() << ", " << center.GetY() << " with area " << area << std::endl;
}

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "usage: " << argv[0] << " background.ppm" << std::endl;
    return 0;
  }
  
  Image image;
  if (argc >= 2)
    image.LoadPPM(argv[1]);

  Segmentation seg(image);
  // tree
  seg.AddHueSegment(80, 82, 83, 255);
  // moon
  seg.AddHueSegment(160, 41, 42, 233);
  // present main body
  seg.AddHueSegment(240, 213, 213, 179);
  // present bow
  seg.AddHueSegment(240, 128, 128, 210);
  
  seg.ClosingOperation();
  
  Image label;
  seg.GetLabelImage(label);
  
  print_area_center(seg, "Moon", 160);
  print_area_center(seg, "Tree", 80);
  print_area_center(seg, "Present", 240);
  
  const int mark_label = 80;
  Coordinate point(seg.GetLabelTopLeft(mark_label));
  std::vector<int> freeman_code;
  seg.GetFreemanCode(mark_label, point, freeman_code);
  seg.DrawContourFreeman(point, freeman_code, Color::red(), label);

  label.SavePPM("out_label.ppm");
  std::cout << "saved out_label.ppm" << std::endl;
    
  return 0;
}
