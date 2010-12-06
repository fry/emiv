#include <Graphics2D/Filter.hh>

using namespace Graphics2D;

namespace {
  int bino(int n, int k) {
    if (k == 0 || k == n) return 1;
    return bino(n - 1, k - 1) + bino(n - 1, k);
  }
}

Filter::Filter(const std::vector<std::vector<int> >& mask): mask_(mask) {
  width_ = mask_.size();
  height_ = mask_[0].size();
  half_width_ = width_ / 2;
  half_height_ = height_ / 2;
  
  // Calculate filter sum
  sum_mask_ = 0;
  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      sum_mask_ += mask_[x][y];
    }
  }
}

void Filter::FilterImage(const Image& src, Image& dst) {  
  for (int x = half_width_; x < src.GetWidth() - half_width_; x++) {
    for (int y = half_height_; y < src.GetHeight() - half_height_; y++) {
      for (int c = 0; c < 3; c++) {
        int sum = 0;
        for (int fx = 0; fx < width_; fx++) {
          for (int fy = 0; fy < height_; fy++) {
            // Center filter around x/y
            const int px = x + fx - half_width_;
            const int py = y + fy - half_height_;          
            sum += mask_[fx][fy] * src.GetPixel(px, py, c);
          }
        }
      
        dst.SetPixel(x, y, c, sum / sum_mask_);
      }
    }
  }
}

Filter* Filter::CreateMean(int width, int height) {
  std::vector<std::vector<int> > mask(width, std::vector<int>(height, 1));
  return new Filter(mask);
}

Filter* Filter::CreateBinomial(int width) {
  // Calculate binomial coefficients
  int binos[width];
  for (int i = 0; i < width; i++) {
    binos[i] = ::bino(width, i);
  }
  
  // Fill matrix
  std::vector<std::vector<int> > mask(width, std::vector<int>(width));  
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < width; y++) {
      mask[x][y] = binos[x] * binos[y];
    }
  }
  
  return new Filter(mask);
}