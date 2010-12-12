#ifndef COLORCONVERSION_HH
#define COLORCONVERSION_HH

#include <Graphics2D/Image.hh>

namespace Graphics2D {
  class ColorConversion {
  public:
    static void ToGrey(const Image& src, Image& dst);
    static void ToHSV(const Image& src, Image& dst);
    static void ToRGB(const Image& src, Image& dst);
  };
}

#endif
