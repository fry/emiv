#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/StructureTensor.hh>

using namespace Graphics2D;

int main(int argc, char** argv) {
  Image img;
  img.LoadPPM(argv[1]);
  img.SetColorModel(ImageBase::cm_Grey);
  
  StructureTensor t;
  t.SetFromImage(img);
  
  t.FoerstnerDetector(0.0005f, img);
  img.SavePPM("foerstner.ppm");
  
  Image out;
  t.HarrisCornerDetector(0.0001, out);
  out.SavePPM("harris.ppm");
  return 0;
}
