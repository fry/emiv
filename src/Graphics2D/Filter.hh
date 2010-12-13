#include <Graphics2D/Image.hh>

#include <vector>

namespace Graphics2D {
  class Filter {
  public:
    void FilterImage(const Image& src, Image& dst);
    static void MeanRecursive(const Image& src, Image &dst, int width, int height);
    static Filter* CreateMean(int width, int height);
    static Filter* CreateBinomial(int width);
    static Filter* CreateGradX();
    static Filter* CreateGradY();
    static Filter* CreateLaplace();
    
    static void FilterGradMag(const Image& src, Image& dst);
    static void Rank3x3(const Image& src, Image& dst, int rank = 3);
  protected:
    Filter(const std::vector<std::vector<int> >& mask, int scale = 1);
    
    std::vector<std::vector<int> > mask_;
    
    int width_, height_, sum_mask_, half_width_, half_height_, scale_;
  };
}