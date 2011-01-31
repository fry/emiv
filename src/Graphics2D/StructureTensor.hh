#ifndef STRUCTURETENSOR_HH_
#define STRUCTURETENSOR_HH_
#include <Graphics2D/Image.hh>
#include <Graphics2DBase/FloatImage.hh>
#include <Graphics2D/Filter.hh>
#include <Graphics2D/Coordinate.hh>

namespace Graphics2D {

  class StructureTensor {
    public:
      StructureTensor(int filterSize=7);
      virtual ~StructureTensor();
      
      void SetFromImage(const Image &input);
      
      void FoerstnerDetector(float thres, Image &corners);
      
      void HarrisCornerDetector(std::vector<Coordinate>& corners);
      
      inline void GetStructureTensor(FloatImage &Jxx, FloatImage &Jxy, FloatImage &Jyy) const {
        Jxx = gxx_;
        Jxy = gxy_;
        Jyy = gyy_;
      }
      
      inline int GetWidth() const { return gxx_.GetWidth(); }
      inline int GetHeight() const { return gxx_.GetHeight(); }
      
    private:
      FloatImage gxx_,gxy_,gyy_;
      
      Filter *filterGx_, *filterGy_, *filterBinomial_;
      
      int halfwinSize_;
  };

}

#endif /* STRUCTURETENSOR_HH_ */
