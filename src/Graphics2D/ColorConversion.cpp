#include <Graphics2D/Image.hh>
#include <Graphics2D/ColorConversion.hh>

#include <stdexcept>
using namespace Graphics2D;

void ColorConversion::ToGrey(const Image& src, Image& dst) {
  Image img;
  if (src.GetColorModel() == ImageBase::cm_Grey) {
    dst = src;
    return;
  } else if (src.GetColorModel() == ImageBase::cm_HSV) {
    img.Init(src.GetWidth(), src.GetHeight());
    ColorConversion::ToRGB(src, img);
  } else
    img = src;

  if (!img.Valid())
    throw std::runtime_error("uninitialized image");
    
  for (int x = 0; x < src.GetWidth(); x++) {
    for (int y = 0; y < src.GetHeight(); y++) {
      unsigned char val = static_cast<unsigned char>(
                           (30 * img.GetPixel(x, y, 0)) / 100 +
                           (59 * img.GetPixel(x, y, 1)) / 100 +
                           (11 * img.GetPixel(x, y, 2)) / 100);
      dst.SetPixel(x, y, 0, val);
      dst.SetPixel(x, y, 1, val);
      dst.SetPixel(x, y, 2, val);
    }
  }
  
  dst.SetColorModel(ImageBase::cm_Grey);
}

void ColorConversion::ToHSV(const Image& src, Image& dst) {
  if (src.GetColorModel() == ImageBase::cm_HSV)
    dst = src;

  if (!src.Valid())
    throw std::runtime_error("uninitialized image");
    
  for (int x = 0; x < src.GetWidth(); x++) {
    for (int y = 0; y < src.GetHeight(); y++) {
      unsigned char r, g, b;
      r = src.GetPixel(x, y, 0);
      g = src.GetPixel(x, y, 1);
      b = src.GetPixel(x, y, 2);
      
      unsigned char max = std::max(r, std::max(g, b));
      unsigned char min = std::min(r, std::min(g, b));
      
      unsigned char chroma = max - min;
      
      // Hue
      float hue_a = 0;
      if (max == r)
        hue_a = static_cast<float>(g - b) / chroma;
      else if (max == g)
        hue_a = 2 + static_cast<float>(b - r) / chroma;
      else if (max == b)
        hue_a = 4 + static_cast<float>(r - g) / chroma;
      
      int hue = 60 * hue_a;
      if (hue < 0)
        hue += 360;
      
      dst.SetPixel(x, y, 0, static_cast<unsigned char>(hue / 360.0 * 256));
      
      // Saturation
      unsigned char saturation;
      if (max == 0)
        saturation = 0;
      else
        saturation = static_cast<unsigned char>(static_cast<float>(max - min) / max * 255);
      
      dst.SetPixel(x, y, 1, saturation);
      
      // Value
      dst.SetPixel(x, y, 2, max);
    }
  }
  
  dst.SetColorModel(ImageBase::cm_HSV);
}

void ColorConversion::ToRGB(const Image& src, Image& dst) {
  if (src.GetColorModel() == ImageBase::cm_RGB || src.GetColorModel() == ImageBase::cm_Grey)
    dst = src;

  if (!src.Valid())
    throw std::runtime_error("uninitialized image");
    
  for (int x = 0; x < src.GetWidth(); x++) {
    for (int y = 0; y < src.GetHeight(); y++) {
      const float v = src.GetPixel(x, y, 2) / 255.0;
      const float s = src.GetPixel(x, y, 1) / 255.0;
      const float h = src.GetPixel(x, y, 0) / 256.0 * 360;
      const int hi = static_cast<int>(h / 60.0);
      const float f = h / 60.0 - hi;
      
      const float p = v * (1 - s);
      const float q = v * (1 - s * f);
      const float t = v * (1 - s * (1 - f));
      
      float r, g, b;
      switch(hi) {
      case 0:
      case 6:
        r = v; g = t; b = p;
        break;
      case 1:
        r = q; g = v; b = p;
        break;
      case 2:
        r = p; g = v; b = t;
        break;
      case 3:
        r = p; g = q; b = v;
        break;
      case 4:
        r = t; g = p; b = v;
        break;
      case 5:
        r = v; g = p; b = q; 
        break;
      default:
        throw std::runtime_error("invalid H value");
      }
      
      dst.SetPixel(x, y, 0, r * 255);
      dst.SetPixel(x, y, 1, g * 255);
      dst.SetPixel(x, y, 2, b * 255);
    }
  }
  
  dst.SetColorModel(ImageBase::cm_RGB);
}