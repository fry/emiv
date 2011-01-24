/* color conversion from different color space models, slow and fast methods
 * @author fkellner 10/10
 */
#ifndef COLORCONVERSION_HH_
#define COLORCONVERSION_HH_

#include <Graphics2D/Image.hh>

namespace Graphics2D {

  class ColorConversion {
    public:

      static int ToRGB(const Image &src, Image &dst);
      static int ToHSV(const Image &src, Image &dst);
      static int ToGrey(const Image &src, Image &dst);

  };

}

#endif /* COLORCONVERSION_HH_ */
