/*
 * Image Class implementing ImageBase
 * @author fkellner, 10/10
 */

#ifndef IMAGE_HH_
#define IMAGE_HH_

#include <Graphics2DBase/ImageBase.hh>

namespace Graphics2D {

  class Image : public ImageBase {
    public:

      Image();
      
      virtual ~Image();
      
      virtual void FillZero();
      
      virtual inline unsigned char GetPixel(const int &x, const int &y, const int &ch) const {
        return data_[3*(y*width_+x)+ch];
      }
      
      virtual inline void SetPixel(const int &x, const int &y, const int &ch, const unsigned char &value) {
        data_[3*(y*width_+x)+ch] = value;
      }
      
      virtual int LoadPPM(const std::string &filename);
      virtual int SavePPM(const std::string &filename);

      
  };

}

#endif /* IMAGE_HH_ */
