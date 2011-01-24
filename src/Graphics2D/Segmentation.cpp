#include <Graphics2D/Segmentation.hh>
#include <Graphics2D/ColorConversion.hh>
#include <Graphics2D/Filter.hh>
#include <Graphics2D/PrimitivePoint.hh>

namespace Graphics2D {

  Segmentation::Segmentation(const Image &inputImage) {
    width_ = inputImage.GetWidth();
    height_ = inputImage.GetHeight();
    
    switch (inputImage.GetColorModel()) {
      case ImageBase::cm_RGB:
        ColorConversion::ToHSV(inputImage, hsvImage_);
        labelImage_.Init(width_, height_);
        labelImage_.SetColorModel(ImageBase::cm_Grey);
        labelImage_.FillZero();
        break;
      case ImageBase::cm_HSV:
        hsvImage_ = inputImage;
        labelImage_.Init(width_, height_);
        labelImage_.SetColorModel(ImageBase::cm_Grey);
        labelImage_.FillZero();
        break;
      case ImageBase::cm_Grey:
        labelImage_ = inputImage;
        break;
        
    }
  }

  Segmentation::~Segmentation() {

  }

  void Segmentation::AddHueSegment(const int label, const int minHue, const int maxHue, const int minSat) {
    if (!hsvImage_.Valid()) {
      std::cout << "input image is a label image already" << std::endl;
      return;
    }
    int offset=0;
    if (minHue>maxHue) {
      offset = 255;
    }
    unsigned char *labels = labelImage_.GetData();
    unsigned char *hsv = hsvImage_.GetData();
    for (unsigned int x=0;x<width_*height_*3;x+=3) {
      if (hsv[x+0]+offset >= minHue && hsv[x+0]+offset <= maxHue+offset && hsv[x+1]>=minSat) {
        labels[x+0] = label;
        labels[x+1] = label;
        labels[x+2] = label;
      }
    }
  }
  
  void Segmentation::GetLabelImage(Image &labelImage) {
    labelImage = labelImage_;
  }

  
  void Segmentation::ClosingOperation() {
    Image tmp;
    Filter::Rank3x3(labelImage_, tmp, 8);
    Filter::Rank3x3(tmp, labelImage_, 0);
  }

  int Segmentation::GetCenterAndArea(int label, Coordinate &center, int &area) {
    center.Set(0.0f,0.0f);
    area = 0;
    for (unsigned int y=0;y<height_;y++) {
      for (unsigned int x=0;x<width_;x++) {
        if (labelImage_.GetPixel(x,y,0) == label) {
          area++;
          center = center+Coordinate(x,y);
        }
      }
    }
    if (area == 0) {
      std::cout << "could not find pixels labeled " << label << std::endl; 
      return 1;
    } else {
      center = center * (1.0f/(float)area);
      return 0;
    }
  }
  
  float Segmentation::GetCircumference(const std::vector<int> &freeman) {
    float ret = 0;
    float sqrt2 = sqrt(2.0); 
    for (int i=0;i<(int)freeman.size();i++) {
      if (freeman[i] % 2 == 0) {
        ret++;
      } else {
        ret += sqrt2;
      }
    }
    return ret;
  }
  
  int Segmentation::GetFreemanCode(int label, Coordinate &firstPoint, std::vector<int> &freemanCode) {
    
    int searchDir = 6; // start south
    
    // find first
    bool stop = false;
    firstPoint.Set(-1,-1);
    freemanCode.clear();
    for (unsigned int y = 0; y < height_;y++) {
      for (unsigned int x = 0; x < width_;x++) {
        if (labelImage_.GetPixel(x,y,0) == label) {
          firstPoint = Coordinate(x,y);
          stop = true;
          break;
        }
      }
      if (stop) break;
    }
    if (firstPoint.x() == -1) {
      std::cout << "could not find pixels labeled " << label << std::endl; 
      return 1;
    }

    std::vector<int> possibleDirs(3);
    
    Coordinate curPoint = firstPoint;
    Coordinate nextPoint = Coordinate(-1,-1);
    
    while (nextPoint.x() != firstPoint.x() || nextPoint.y() != firstPoint.y()) {
      
      possibleDirs[0] = (searchDir - 1 + 8) % 8;
      possibleDirs[1] = (searchDir    ) % 8;
      possibleDirs[2] = (searchDir + 1) % 8;
      
      int contourDir = -1;
      for (int i=0;i<3;i++) {
        int x = curPoint.x();
        int y = curPoint.y();
        GetFreemanDirXY_(possibleDirs[i], x,y);
        if (labelImage_.GetPixel(x,y,0) == label) {
          nextPoint = Coordinate(x,y);
          contourDir = possibleDirs[i];
          break;
        }
      }
      if (contourDir == searchDir || contourDir == (searchDir+1)%8) {
        // searchDir stays same
        curPoint = nextPoint;
        freemanCode.push_back(contourDir);
      } else if (contourDir == (searchDir - 1 + 8)%8) {
        searchDir = (searchDir - 2 + 8) % 8;
        curPoint = nextPoint;
        freemanCode.push_back(contourDir);
      } else {
        searchDir = (searchDir + 2) % 8;
      }
    }

    return 0;
  }
  
  
  void Segmentation::DrawContourFreeman(const Coordinate firstPoint, const std::vector<int> &freemanCode, 
      const Color color, Image &targetImage) {
    PrimitivePoint p;
    Coordinate coo = firstPoint;
    p.SetCoordinate(coo);
    p.SetColor(color);
    p.Draw(&targetImage);
    for (unsigned int i=0;i<freemanCode.size();i++) {
      int x = coo.x();
      int y = coo.y();
      GetFreemanDirXY_(freemanCode[i], x, y);
      coo.Set(x,y);
      p.SetCoordinate(coo);
      p.Draw(&targetImage);
    }
  }
  
  void Segmentation::GetAbsoluteCoordinates(const Coordinate &firstPoint, const std::vector<int> &freemanCode, 
      std::vector<Coordinate> &coords) {
    coords.clear();
    coords.push_back(firstPoint);
    Coordinate coo = firstPoint;
    for (unsigned int i=0;i<freemanCode.size();i++) {
      int x = coo.x();
      int y = coo.y();
      GetFreemanDirXY_(freemanCode[i], x, y);
      coo.Set(x,y);
      coords.push_back(coo);
    }
  }


  
  int Segmentation::GetFreemanDirXY_(int dir, int &x, int &y) {
    switch (dir) {
      case 0: // east
        x++;
        break;
      case 1: // north east
        y--;
        x++;
        break;
      case 2: // north
        y--;
        break;
      case 3: // north west
        y--;
        x--;
        break;
      case 4: // west
        x--;
        break;
      case 5: // south west
        y++;
        x--;
        break;
      case 6: // south
        y++;
        break;
      case 7: // south east
        x++;
        y++;
        break;
      default:
        std::cout << "non valid direction" << std::endl;
        return 1;
    }
    return 0;
  }

}
