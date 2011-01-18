/**
 * Filter Factory for NxN filters. Non-generic filters are realized by static methods.
 * 
 * New: Filtering of FloatImages, please read comments below
 * 
 * @author fkellner 12/10 
 */
#ifndef FILTER_HH_
#define FILTER_HH_

#include <vector>
#include <Graphics2D/Image.hh>
#include <Graphics2DBase/FloatImage.hh>

namespace Graphics2D {

  class Filter {
    public:
      /// returns a widthxheight mean filter
      static Filter* CreateMean(int width, int height);
      /// returns a widthxwidth binomial filter
      static Filter* CreateBinomial(int width);

      /// fast implementation of mean filter, reusing partial sums in filter window
      static void MeanRecursive(const Image& src, Image &dst, int width, int height);

      /// returns a central difference gradient filter, x direction
      static Filter* CreateGradX();
      /// returns a central difference gradient filter, y direction
      static Filter* CreateGradY();
      /// returns a laplace filter (2nd derivative)
      static Filter* CreateLaplace();
      
      /// magnitude of gradients filter, sums up x and y gradients
      static void GradMag(const Image& src, Image &dst);
      
      /// 3x3 rank filter, rank=4 is median filter, rank=0 erosion, rank=8 dilation
      static void Rank3x3(const Image& src, Image &dst, int rank=4);

      /// filter image src, store result in dst
      void FilterImage(const Image& src, Image &dst);

      /**
       * Filtering a Greyscale Image to a one-channel FloatImage
       * It is not necessary to shift, scale or discretize the result, so it is often better suited for image processing.
       * 
       * Usage example:
       * Image rgbImage, greyImage;
       * FloatImage gx;
       * 
       * rgbImage.LoadPPM("myholidaypic.ppm");
       * ColorConversion::ToGrey(rgbImage, greyImage);
       * 
       * Filter *filterGradX = Filter::CreateGradX();
       * filterGradX->FilterImage(greyImage, gx);
       * delete filterGradX;
       * gx.GetAsGreyImage(greyImage);
       * greyImage.SavePPM("myholidaypic-gradX.ppm");
       * 
       * @param src Input image, will be scaled from [0..255] to [0.0f..1.0f]!
       * @param dst float valued output image (any range!)
       */
      void FilterImage(const Image& src, FloatImage &dst);
      
      /**
       * Filtering from FloatImage to FloatImage
       * @param src input image
       * @param dst output image
       */
      void FilterImage(const FloatImage& src, FloatImage &dst);

      virtual ~Filter();
    protected:
      /// filters can only be created by factory
      Filter(const std::vector< std::vector<int> > &mask, int scale=0);

    private:
      /// filter mask, stored row-major order
      std::vector< std::vector<int> > mask_;
      /// sum of filter mask
      int sum_;
      /// width and height of filter mask
      unsigned int width_, height_;
      /// half mask width
      int hmw_;
      /// half mask height
      int hmh_;
      /// offset and scale for highpass filters
      int offset_, scale_;
   
      static int Binomial_(int n, int k);
  };

}

#endif /* FILTER_HH_ */
