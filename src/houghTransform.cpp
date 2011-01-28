#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>

using namespace Graphics2D;

int main(int argc, char** argv) {
  Image img;
  img.LoadPPM(argv[1]);
  img.SetColorModel(ImageBase::cm_Grey);
  
  std::vector<PrimitiveLine> out; out.clear();
  HoughTransform trans;
  trans.StandardHoughTransform(img, 2, out);
  return 0;
}
