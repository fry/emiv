#ifndef IMAGEBASE_HH_
#define IMAGEBASE_HH_

#include <string>
#include <iostream>

// our library will live in Graphics2D namespace
namespace Graphics2D {

  class ImageBase {
    public:
      
      // each image has a colormodel, defaults to rgb
      enum ColorModel {
        cm_RGB=0,
        cm_HSV,
        cm_Grey
      };
      
      // creates empty image
      ImageBase();
      // copies image from other instance
      ImageBase(const ImageBase &other);
      // copies image from other instance
      ImageBase& operator=(const ImageBase &other);

      // deletes the image
      virtual ~ImageBase();
      
      // creates memory for a width*height 3 channel image
      void Init(unsigned int width, unsigned int height);
      
      // returns true if image is inited
      bool Valid() const;
      
      // sets every pixel to black ( rgb(0,0,0) ), must be implemented by child 
      virtual void FillZero() = 0;
      
      // returns color channel value of pixel at (x,y) position, must be implemented by child
      virtual inline unsigned char GetPixel(const int &x, const int &y, const int &ch) const = 0;
      
      // set a value for color channel ch for pixel (x,y)
      virtual inline void SetPixel(const int &x, const int &y, const int &ch, const unsigned char &value) = 0;

      // load from a ppm image file, loads p3 and p6 images
      virtual int LoadPPM(const std::string &filename) = 0;
      // save as a ppm p6 image file
      virtual int SavePPM(const std::string &filename) = 0;

      // get pointer to data
      inline unsigned char *GetData() { return data_; }
      inline unsigned char *GetData() const { return data_; }
      // get image width
      inline unsigned int GetWidth() const { return width_; }
      // get image height
      inline unsigned int GetHeight() const { return height_; }
      // get color model
      inline ColorModel GetColorModel() const { return colormodel_; }
      // set color model
      inline void SetColorModel(ColorModel colormodel) { colormodel_ = colormodel; }

    protected:
      
      unsigned int width_, height_;
      ColorModel colormodel_;
      unsigned char *data_;

  };

}

#endif /* IMAGEBASE_HH_ */
