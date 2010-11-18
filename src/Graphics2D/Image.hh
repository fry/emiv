// Gruppe 8
// Serie 2
// Alexander Koch, 773036
// Eike Siewertsen, 847522

#ifndef IMAGE_HH
#define IMAGE_HH

#include <Graphics2DBase/ImageBase.hh>
#include <cassert>

namespace Graphics2D {
  class Image: public ImageBase {
  public:
    void FillZero();
  
    inline unsigned char GetPixel(const int &x, const int &y, const int &ch) const {
      assert(ch >= 0 && ch <= 2);
      assert(x >= 0 && x <= width_);
      assert(y >= 0 && y <= height_);

      return data_[(width_ * y + x) * 3 + ch];
    }

    inline void SetPixel(const int &x, const int &y, const int &ch, const unsigned char &value) {
      assert(ch >= 0 && ch <= 2);
      assert(x >= 0 && x <= width_);
      assert(y >= 0 && y <= height_);

      data_[(width_ * y + x) * 3 + ch] = value;
    }

    int LoadPPM(const std::string &filename);
    int SavePPM(const std::string &filename);
  };
}


#endif

