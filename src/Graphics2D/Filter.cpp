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

//void recursive_helper(const Image& src, Image &dst, int width, int height)
void Filter::MeanRecursive(const Image& src, Image &dst, int width, int height){
	int half_width = width/2;
	int half_height = height/2;
	
	Image tmp;
	tmp.Init(src.GetWidth(), src.GetHeight());
	
	//for each color
	for (int c=0; c<3; c++){
		//run through all the rows
		for (int pic_row=0; pic_row<src.GetHeight();pic_row++){
			//initialize the zw_sum excluduing the last pixel
			int zw_sum=0;
			for (int filter=0; filter<width-1; filter++){
				zw_sum+=src.GetPixel(filter, pic_row, c);
			}
			//run through all the pixels we can update (leave out the border)
			for (int pic_col=half_width; pic_col < src.GetWidth()-half_width; pic_col++){
				//add the last pixel
				zw_sum+=src.GetPixel(pic_col + half_width, pic_row, c);
				tmp.SetPixel(pic_col, pic_row, c, zw_sum/width);
				//remove the first pixel
				zw_sum-=src.GetPixel(pic_col - half_width, pic_row, c);
			}
		}
		
		//at the end run through all the cols and do the same thing
		for (int pic_col=0; pic_col<src.GetWidth();pic_col++){
			//initialize the zw_sum excluduing the last pixel
			int col_zw_sum=0;
			for (int filter=0; filter<height-1; filter++){
				col_zw_sum+=tmp.GetPixel(pic_col, filter, c);
			}
			//run through all the pixels we can update (leave out the border)
			for (int pic_row=half_height; pic_row < src.GetHeight()-half_height; pic_row++){
				//add the last pixel
				col_zw_sum+=tmp.GetPixel(pic_col, pic_row + half_height, c);
				dst.SetPixel(pic_col, pic_row, c, col_zw_sum/width);
				//remove the first pixel
				col_zw_sum-=tmp.GetPixel(pic_col, pic_row - half_height, c);
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