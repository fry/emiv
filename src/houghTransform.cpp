#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/HoughTransform.hh>

using namespace Graphics2D;

void save_image_lines(const std::string& file_name, const Image& img, std::vector<PrimitiveLine>& lines) {
  Image out(img);

  for (std::vector<PrimitiveLine>::iterator iter=lines.begin(); iter != lines.end(); ++iter) {
    iter->SetColor(Color::red());
    iter->Draw(&out);
  }
  
  out.SavePPM(file_name); 
}

int main(int argc, char** argv) {
  Image img;
  img.LoadPPM(argv[1]);
  img.SetColorModel(ImageBase::cm_Grey);
  
  std::vector<PrimitiveLine> lines; lines.clear();
  HoughTransform trans;
  trans.StandardHoughTransform(img, 2, lines);
  save_image_lines("lines_standard.ppm", img, lines);
  
  StructureTensor tens;
  tens.SetFromImage(img);
  
  trans.FastHoughTransform(tens, 2, lines);
  save_image_lines("lines_fast.ppm", img, lines);
  
  return 0;
}
