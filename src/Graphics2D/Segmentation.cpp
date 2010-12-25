#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/ColorConversion.hh>
#include <Graphics2D/Filter.hh>

using namespace Graphics2D;

namespace {
  inline int fmc(int fm) {
    return (fm + 8) % 8;
  }
  
  void get_fm(int code, int& dx, int& dy) {
    code = fmc(code);
    dx = 0; dy = 0;
    switch(code) {
      case 0:
        dx = 1;
        break;
      case 1:
        dx = 1; dy = -1;
        break;
      case 2:
        dy = -1;
        break;
      case 3:
        dx = -1; dy = -1;
        break;
      case 4:
        dx = -1;
        break;
      case 5:
        dx = -1; dy = 1;
        break;
      case 6:
        dy = 1;
        break;
      case 7:
        dx = 1; dy = 1;
        break;
    }
  }
}

Segmentation::Segmentation(const Image &inputImage) {
  hsvImage_.Init(inputImage.GetWidth(), inputImage.GetHeight());
  labelImage_.Init(inputImage.GetWidth(), inputImage.GetHeight());
  
  const ImageBase::ColorModel cm = inputImage.GetColorModel();
  if (cm == ImageBase::cm_RGB) {
    ColorConversion::ToHSV(inputImage, hsvImage_);
    labelImage_.FillZero();
  } else if (cm == ImageBase::cm_HSV) {
    hsvImage_ = inputImage;
    labelImage_.FillZero();
  } else {
    labelImage_ = inputImage;
  }
  
  labelImage_.SetColorModel(ImageBase::cm_Grey);
}

Segmentation::~Segmentation() {
  
}

void Segmentation::AddHueSegment(const int label, const int minHue, const int maxHue, const int minSat) {
  if (!hsvImage_.Valid())
    return;

  for (int x = 0; x < hsvImage_.GetWidth(); x++) {
    for (int y = 0; y < hsvImage_.GetHeight(); y++) {
      const int h = hsvImage_.GetPixel(x, y, 0);
      const int s = hsvImage_.GetPixel(x, y, 1);
      const int v = hsvImage_.GetPixel(x, y, 2);
      
      if (s >= minSat && h >= minHue && h <= maxHue) {
        labelImage_.SetPixel(x, y, 0, label);
        labelImage_.SetPixel(x, y, 1, label);
        labelImage_.SetPixel(x, y, 2, label);
      }
    }
  }
}

void Segmentation::ClosingOperation() {
  Image tmp; tmp.Init(labelImage_.GetWidth(), labelImage_.GetHeight());
  Filter::Rank3x3(labelImage_, tmp, 8);
  Filter::Rank3x3(tmp, labelImage_, 0);
}

void Segmentation::GetLabelImage(Image &labelImage) {
  labelImage = labelImage_;
}

int Segmentation::GetCenterAndArea(const int label, Coordinate &center, int &area) {
  area = 0;
  center = Coordinate();
  for (int x = 0; x < labelImage_.GetWidth(); x++) {
    for (int y = 0; y < labelImage_.GetHeight(); y++) {
      const int current_label = labelImage_.GetPixel(x, y, 0);
      if (current_label == label) {
        center += Coordinate(x, y);
        area ++;
      }
    }
  }
  
  center *= 1.0f / area;
}

int Segmentation::GetFreemanCode(const int label, const Coordinate &firstPoint, std::vector<int> &freemanCode) {
  int cx = firstPoint.GetX();
  int cy = firstPoint.GetY();
  
  const int width = labelImage_.GetWidth();
  const int height = labelImage_.GetHeight();
  
  const Color color = Color::red();
  
  int cb = 6;
  while (true) {
    int ck, dx, dy;
    bool found = false;
    for (ck = cb - 1; ck <= cb + 1; ck++) {
      get_fm(ck, dx, dy);
      const int px = cx + dx;
      const int py = cy + dy;
      if (px >= 0 && py >= 0 && px <= width && py <= height) {
        if (labelImage_.GetPixel(px, py, 0) == label) {
          cx = px; cy = py;
          found = true;
          break;
        }
      }
    }
    
    ck = fmc(ck);
    
    if (ck == cb || ck == fmc(cb + 1)) {
      freemanCode.push_back(ck);
    } else if (ck == fmc(cb - 1)) {
      cb -= 2;
      freemanCode.push_back(ck);
    } else {
      cb += 2;
    }
    
    cb = ((cb + 8) % 8);
    
    if (cx == firstPoint.GetX() && cy == firstPoint.GetY())
      break;
  }
}

void Segmentation::DrawContourFreeman(const Coordinate& firstPoint, const std::vector<int> &freemanCode, 
    const Color color, Image &targetImage) {
  int cx = firstPoint.GetX();
  int cy = firstPoint.GetY();
  
  std::vector<int>::const_iterator iter, end;
  end = freemanCode.end();
  
  int i = 0;
  for (iter = freemanCode.begin(); iter != end; ++iter) {
    int dx, dy;
    get_fm(*iter, dx, dy);
    cx += dx; cy += dy;
    
    if (cx < 0 || cy < 0)
      break;
    
    targetImage.SetPixel(cx, cy, 0, color.GetRed());
    targetImage.SetPixel(cx, cy, 1, color.GetGreen());
    targetImage.SetPixel(cx, cy, 2, color.GetBlue());
  }
}
