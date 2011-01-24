
#include <Graphics2D/ColorConversion.hh>
#include <cmath>
#include <algorithm>

using namespace std;

namespace Graphics2D {

  int ColorConversion::ToRGB(const Image &src, Image &dst) {
    
    if (src.GetColorModel() == Image::cm_RGB) {
      // if already rgb, just copy to dst
      dst = src;
      return 0;
    } 
    if (src.GetColorModel() == Image::cm_Grey) {
      cout << "can not convert from grey to rgb" << endl;
      return -1;
    }

    unsigned int w=src.GetWidth();
    unsigned int h=src.GetHeight();
    dst.Init(w,h);
    
    const unsigned char *idaSrc = src.GetData();
    unsigned char *idaDst = dst.GetData();

    unsigned int xs=0;
    unsigned int xd=0;
    
    while (xs<w*h*3) {
      float p1, p2, p3, i, f;
      float xh;
      
      // get old values and convert to float [0..1] (for hue it is [0..1) )
      float h = (float)idaSrc[xs++] / 256.0f;
      float s = (float)idaSrc[xs++] / 255.0f;
      float v = (float)idaSrc[xs++] / 255.0f;

      xh = h * 6.0f;                  /* convert hue to be in 0,6       */
      i = (float)floor((double)xh);    /* i = greatest integer <= h    */
      f = xh - i;                      /* f = fractional part of h     */
      p1 = v * (1.0f - s);
      p2 = v * (1.0f - (s * f));
      p3 = v * (1.0f - (s * (1.0f - f)));
      
      float r,g,b;
      
      switch ((int) i)
      {
      case 0:
        r = v; g = p3;  b = p1; break;
      case 1:
        r = p2; g = v;  b = p1; break;
      case 2:
        r = p1; g = v;  b = p3; break;
      case 3:
        r = p1; g = p2; b = v;  break;
      case 4:
        r = p3; g = p1; b = v;  break;
      case 5:
        r = v; g = p1; b = p2; break;
      default:
        r = 0.0f; g = 0.0f; b = 0.0f;
        break;
      }
      
      /* Normalize the values to 255 */
      r *= 255.0f;
      g *= 255.0f;
      b *= 255.0f;
      idaDst[xd++] = (unsigned char)r;
      idaDst[xd++] = (unsigned char)g;
      idaDst[xd++] = (unsigned char)b;
    }
    dst.SetColorModel(Image::cm_RGB);
    return 0;
  }
  
  int ColorConversion::ToHSV(const Image &src, Image &dst) {
    if (src.GetColorModel() == Image::cm_HSV) {
      // just copy to dst
      dst = src;
      return 0;
    }
    if (src.GetColorModel() == Image::cm_Grey) {
      cout << "can not convert from grey to hsv" << endl;
      return -1;
    }
    unsigned int w=src.GetWidth();
    unsigned int h=src.GetHeight();
    dst.Init(w,h);
    
    float hueSixth = 256.0f / 6.0f;
    
    const unsigned char *idaSrc = src.GetData();
    unsigned char *idaDst = dst.GetData();

    unsigned int xs=0;
    unsigned int xd=0;
    
    while (xs<w*h*3) {

      float r = (float)idaSrc[xs++] / 255.0f;
      float g = (float)idaSrc[xs++] / 255.0f;
      float b = (float)idaSrc[xs++] / 255.0f;
      
      float h,s,v;

      float maxrgb = max(r, max(g, b));
      float minrgb = min(r, min(g, b));

      // hue
      if (minrgb == maxrgb) {
        h = 0.0f;
      }
      else if (maxrgb == r) {
        h = hueSixth * ((g - b) / (maxrgb - minrgb));
      }
      else if (maxrgb == g) {
        h = hueSixth * (2.0f + (b - r) / (maxrgb - minrgb));
      }
      else {
        h = hueSixth * (4.0f + (r - g) / (maxrgb - minrgb));
      }
      if (h < 0.0f) {
        h += 256.0f;
      }

      // sat
      if (maxrgb == 0) {
        s = 0.0f;
      }
      else {
        s = 255.0f * (maxrgb - minrgb) / maxrgb;
      }

      // value
      v = 255.0f * maxrgb;

      idaDst[xd++] = (unsigned char)h;
      idaDst[xd++] = (unsigned char)s;
      idaDst[xd++] = (unsigned char)v;
    }
    dst.SetColorModel(Image::cm_HSV);

    return 0;
  }

  int ColorConversion::ToGrey(const Image &src, Image &dst) {
    if (src.GetColorModel() == Image::cm_Grey) {
      cout << "grey already" << endl;
      dst = src;
      return 0;
    }
    if (src.GetColorModel() != Image::cm_RGB) {
      ToRGB(src, dst);
      return ToGrey(dst, dst);
    }
    // Y = 0.3 * R + 0.59 * G + 0.11 * B
    unsigned int w=src.GetWidth();
    unsigned int h=src.GetHeight();

    dst.Init(w,h);
    
    const unsigned char *idaSrc = src.GetData();
    unsigned char *idaDst = dst.GetData();

    int d;
    unsigned char dc;
    unsigned int xs=0;
    unsigned int xd=0;
    while (xs<w*h*3) {
      d=0;
      d+= 30 * (int)idaSrc[xs++];
      d+= 59 * (int)idaSrc[xs++];
      d+= 11 * (int)idaSrc[xs++];
      dc = (unsigned char)(d/100);
      idaDst[xd++] = dc; 
      idaDst[xd++] = dc; 
      idaDst[xd++] = dc; 
    }
    dst.SetColorModel(Image::cm_Grey);
    return 0;
  }
  
  
}
