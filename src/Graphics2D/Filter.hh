#include <Graphics2D/Image.hh>

#include <vector>

namespace Graphics2D {
  class Filter {
  public:
    void FilterImage(const Image& src, Image& dst);
    
    static Filter* CreateMean(int width, int height);
    static Filter* CreateBinomial(int width);
  protected:
    Filter(const std::vector<std::vector<int> >& mask);
    
    std::vector<std::vector<int> > mask_;
    
    int width_, height_, sum_mask_, half_width_, half_height_;
  };
}