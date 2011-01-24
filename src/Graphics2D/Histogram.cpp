#include <Graphics2D/Histogram.hh>
#include <Graphics2D/ColorConversion.hh>
#include <Graphics2D/Coordinate.hh>
#include <algorithm>

namespace Graphics2D {

  Histogram::Histogram() {
    histo_.resize(256, 0);
    posx_ = 0;
    posy_ = 0;
    width_ = 0;
    height_ = 0;
  }

  Histogram::~Histogram() {
    // TODO Auto-generated destructor stub
  }

  int Histogram::FromImage(const Image &image, const int &channel) {
    // reset to zero
    for (int i=0;i<256;i++) {
      histo_[i] = 0;
    }
    // start at p=channel. increasing p by 3 on each step will walk the image on correct channel
    int p=channel;
    int w = image.GetWidth();
    int h = image.GetHeight();
    const unsigned char *data = image.GetData();
    while (p<w*h*3) {
      // increase histogram at current value
      if (p%3==0 && data[p+1] > 100)
      histo_[ (int)data[p] ] ++;
      p+= 3;
    }
//    for (unsigned int i=1;i<256;i++) {
//      histo_[i] = histo_[i]+histo_[i-1];
//    }
    return 0;
  }
  
  int Histogram::Autocontrast(const Image &src, Image &dst) {
    // transform image to hsv
    ColorConversion::ToHSV(src, hsvImage_);
    // create histogram for v channel
    FromImage(hsvImage_,2);
    
    // get f(x) = mx+b such that the histogram is spread to 0..255
    float m,b;
    if (GetAutocontrastFunc_(m,b) != 0) {
      dst = src;
      return -1;
    }
    // apply on hsv image
    unsigned char *data = hsvImage_.GetData();
    int p=2;
    int w = hsvImage_.GetWidth();
    int h = hsvImage_.GetHeight();
    while (p<w*h*3) {
      data[p] = (unsigned char) ( m * float(data[p]) +b); 
      p+= 3;
    }

    // convert back to rgb
    ColorConversion::ToRGB(hsvImage_, dst);
    return 0;
  }
  
  int Histogram::GetAutocontrastFunc_(float &m, float &b) {
    int minv = 255;
    int maxv = 0;
    // find minv and maxv by finding first non-zero value
    for (int i=0;i<256;i++) {
      if (histo_[i] != 0) {
        minv = i;
        break;
      }
    }
    for (int i=255;i>=0;i--) {
      if (histo_[i] != 0) {
        maxv = i;
        break;
      }
    }
    // there is not so much contrast to increase, return error
    if (minv >= maxv) {
      std::cout << "can not determine autocontrast function" << std::endl;
      return -1;
    }
    // set m and b
    m = 255.0f / (maxv-minv);
    b = -minv*m;
    return 0;
  }

  void Histogram::SetCoordinates(std::vector<Coordinate> coo) {
    if (coo.size()!=2) {
      std::cout << "histogram has 2 coordinates: upper left and lower right corner" << std::endl;
      return;
    }
    boxes_.resize(256);
    float bwidth = (float)coo[1].x() / 257.0f;
    int bmaxheight = coo[1].y();
    int maxval = *(std::max_element(histo_.begin(), histo_.end()));
    if (maxval == 0) maxval = 1; // failsafe
    float bscale = (float)bmaxheight / log10(maxval+1.0);
//    float bscale = (float)bmaxheight / (maxval);
    int posx = coo[0].x();
    int posy = coo[0].y();
    int lasty = posy+bmaxheight;
    for (unsigned int i=0;i<256;i++) {
      std::vector<Coordinate> coos(2);
      coos[0] = Coordinate(i*bwidth+posx, lasty);
      coos[1] = Coordinate((i+1)*bwidth+posx, lasty-log10(histo_[i]+1.0)*bscale);
//      coos[1] = Coordinate((i+1)*bwidth+posx, lasty-histo_[i]*bscale);
      boxes_[i].SetCoordinates(coos);
      boxes_[i].SetColor(color_);
    }
  }

  void Histogram::Draw(ImageBase *im) {
    for (unsigned int i=0;i<256;i++) {
      boxes_[i].Draw(im);
    }    
  }
}
