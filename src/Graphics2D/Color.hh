/*
 * A color has r,g,b components
 * @author fkellner, 10/10
 */

#ifndef COLOR_HH_
#define COLOR_HH_

#include <iostream>

namespace Graphics2D {

  class Color {
    public:
      /**
       * constructor defaults to color black
       * @brief default constructor
       */
      Color() : r_(0), g_(0), b_(0), name_("black") {}
      
      /**
       * constructor
       *
       * @brief generates color with rgb-values and name
       *
       * @param r red portion of color
       * @param g green portion of color
       * @param b blue portion of color
       * @param name name of color
       */
      Color(unsigned char r,unsigned char g, unsigned char b, std::string name=std::string("")) :
        r_(r), g_(g), b_(b), name_(name) {}
      
      virtual ~Color() {}
      
      // get components
      inline unsigned char r() { return r_; }
      inline unsigned char g() { return g_; }
      inline unsigned char b() { return b_; }
      inline std::string name() { return name_; }
      
      // some colors
      static inline const Color red() { return Color(255,0,0, "red"); }
      static inline const Color green() { return Color(0,255,0, "green"); }
      static inline const Color blue() { return Color(0,0,255, "blue"); }
      static inline const Color black() { return Color(0,0,0, "black"); }
      static inline const Color white() { return Color(255,255,255, "white"); }
      static inline const Color yellow() { return Color(255,255,0, "yellow"); }
    protected:
      unsigned char r_,g_,b_;
      std::string name_;
  };
}

#endif /* COLOR_HH_ */
