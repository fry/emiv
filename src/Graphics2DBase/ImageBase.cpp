#include <string.h>
#include <Graphics2DBase/ImageBase.hh>

namespace Graphics2D {

  ImageBase::ImageBase() {
    // initialize variables as zero
    width_ = 0;
    height_ = 0;
    data_ = NULL;
    colormodel_ = cm_RGB;
  }
  
  ImageBase::ImageBase(const ImageBase &other) {
    // a copy constructor is called whenever a call by value function is called:
    // void dosomething(Image b) { ... }
    // Image a;
    // ...
    // dosomething(a);
    // you can also call it yourself:
    // Image a;
    // ...
    // Image b(a);
    
    // copy everything from other image
    width_ = other.width_;
    height_ = other.height_;
    colormodel_ = other.colormodel_;
    data_ = new unsigned char[width_*height_*3];
    memcpy(data_, other.data_, width_*height_*3);
  }

  ImageBase& ImageBase::operator=(const ImageBase &other) {
    // basically the same as copy constructor, but "this" instance already available
    // usage is more common in user code:
    // Image a;
    // ...
    // Image b = a;
    
    // check if call is like a=a, which means do nothing
    if (this != &other) {
      if (Valid()) {
        if (other.width_ != width_ || other.height_ != height_) {
          delete[] data_;
          data_ = NULL;
        }
      }
      width_ = other.width_;
      height_ = other.height_;
      colormodel_ = other.colormodel_;
      if (!Valid())
        data_ = new unsigned char[width_*height_*3];
      memcpy(data_, other.data_, width_*height_*3);
    }
    return *this;
  }

  ImageBase::~ImageBase() {
    // if data is allocated, delete it
    if (Valid()) {
      delete[] data_;
      data_ = NULL;
    }
  }

  bool ImageBase::Valid() const {
    return data_ != NULL;
  }
  
  void ImageBase::Init(unsigned int width, unsigned int height) {
    // only (re-)allocate data if neccessary
    if (Valid()) {
      if (width != width_ || height != height_) {
        delete[] data_;
        data_ = new unsigned char[width*height*3];
      }
    } else {
      data_ = new unsigned char[width*height*3];
    }
    width_ = width;
    height_ = height;
  }
}
