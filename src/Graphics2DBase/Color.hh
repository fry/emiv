#ifndef COLOR_HH
#define COLOR_HH

#include <string>

namespace Graphics2D {
  class Color {
  public:
    static Color black() {
      return Color("black", 0, 0, 0);
    }

    static Color red() {
      return Color("red", 255, 0, 0);
    }

    static Color green() {
      return Color("green", 0, 255, 0);
    }

    static Color blue() {
      return Color("blue", 0, 0, 255);
    }
    
    static Color white(){
      return Color("white",255,255,255);
    }
    
    static Color yellow(){
      return Color("yellow",255,255,0);
    }

    Color(const std::string& name, unsigned char r, unsigned char g, unsigned char b);
    
    inline const std::string& GetName() const {
      return name_;
    }
    
    inline unsigned char GetRed() const {
      return r_;
    }
    
    inline unsigned char GetGreen() const {
      return g_;
    }
    
    inline unsigned char GetBlue() const {
      return b_;
    }
    
    inline void SetRed(unsigned char r) {
      r_ = r;
    }
    
    inline void SetGreen(unsigned char g) {
      g_ = g;
    }
    
    inline void SetBlue(unsigned char b) {
      b_ = b;
    }
  protected:
    unsigned char r_, g_, b_;
    std::string name_;
  };
}

#endif
