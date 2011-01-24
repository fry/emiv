/*
 * A background image for the painter. just copies a source image into the destination image
 * @author fkellner 10/10 
 */

#ifndef BACKGROUNDIMAGE_HH_
#define BACKGROUNDIMAGE_HH_

#include <Graphics2D/PrimitiveBase.hh>
#include <Graphics2D/Image.hh>

namespace Graphics2D {

  class BackgroundImage : public PrimitiveBase {
    public:
      BackgroundImage(Image &im);
      BackgroundImage(std::string filename);
      virtual ~BackgroundImage();
      
      void Draw(ImageBase *im);
      
    private:
      Image source_;
  };

}

#endif /* BACKGROUNDIMAGE_HH_ */
