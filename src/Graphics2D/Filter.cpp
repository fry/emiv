#include <Graphics2D/Filter.hh>
#include <Graphics2D/ColorConversion.hh>

#include <cmath>

using namespace Graphics2D;

namespace {
  int bino(int n, int k) {
    if (k == 0 || k == n) return 1;
    return bino(n - 1, k - 1) + bino(n - 1, k);
  }
}

Filter::Filter(const std::vector<std::vector<int> >& mask, int scale): mask_(mask), scale_(scale) {
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

        // If the filter sum is zero, use passed scale factor to convert from
        // -scale/2 * 255 .. +scale/2 * 255 to 0 .. 255
        if (sum_mask_ == 0)
          dst.SetPixel(x, y, c, (sum + scale_ / 2 * 255) / scale_);
        else
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

void Filter::Rank3x3(const Image& src, Image& dst, int rank) {
  assert(rank >= 0 && rank < 3*3+1);
  
  // Initialise temporary image to store grayscale version of the source image
  Image tmp;
  tmp.Init(src.GetWidth(), src.GetHeight());
  ColorConversion::ToGrey(src, tmp);
  
  dst.SetColorModel(ImageBase::cm_Grey);
  
  // Place to store color values
  std::vector<int> values;
  values.reserve(3 * 3 + 1);

  for (int x = 1; x < src.GetWidth() - 1; x++) {
    for (int y = 1; y < src.GetHeight() - 1; y++) {
      // Read a 1 pixel environment
      for (int fx = -1; fx <= 1; fx++) {
        for (int fy = -1; fy <= 1; fy++) {
          values.push_back(tmp.GetPixel(x + fx, y + fy, 0));
        }
      }
      
      // Sort the values
      std::sort(values.begin(), values.end());
      
      // From the sorted list, use the pixel at the specified rank as output
      const int current_value = tmp.GetPixel(x, y, 0);
      const int new_value = values[rank];
      dst.SetPixel(x, y, 0, new_value);
      dst.SetPixel(x, y, 1, new_value);
      dst.SetPixel(x, y, 2, new_value);
      
      values.clear();
    }
  }
}

Filter* Filter::CreateGradX() {
  //  0  0  0
  // -1  0  1
  //  0  0  0
  std::vector<std::vector<int> > mask(3, std::vector<int>(1, 0));
  mask[0][0] = -1;
  mask[2][0] = 1;
  
  return new Filter(mask, 2);
}

Filter* Filter::CreateGradY() {
  // 0 -1  0
  // 0  0  0
  // 0  1  0
  std::vector<std::vector<int> > mask(1, std::vector<int>(3, 0));
  mask[0][0] = -1;
  mask[0][2] = 1;
  
  return new Filter(mask, 2);
}

Filter* Filter::CreateLaplace() {
  //  0 -1  0
  // -1  4 -1
  //  0 -1  0
  std::vector<std::vector<int> > mask(3, std::vector<int>(3, 0));
  mask[1][0] = -1;
  mask[0][1] = -1;
  mask[2][1] = -1;
  mask[1][2] = -1;
  mask[1][1] = 4;
  
  return new Filter(mask, 4);
}

void Filter::FilterGradMag(const Image& src, Image& dst) {
  // Filter an image by applying both gradient filters and
  // scaling the resulting magnitude of the vector down to
  // 0..255 again
  
  // Calculate the proper scaling factor for the euclidian metric
  const float mag_factor = 255.0 / std::sqrt(255*255 + 255*255);
  
  for (int x = 1; x < src.GetWidth() - 1; x++) {
    for (int y = 1; y < src.GetHeight() - 1; y++) {
      for (int c = 0; c < 3; c ++) {
        // x grad
        const int dx = -src.GetPixel(x - 1, y, c) + src.GetPixel(x + 1, y, c);
        // y grad      
        const int dy = -src.GetPixel(x, y - 1, c) + src.GetPixel(x, y + 1, c);
        
        // scale down magnitude
        const int mag_real = rint(std::sqrt(dx*dx + dy*dy) * mag_factor);
        dst.SetPixel(x, y, c, mag_real);
      }
    }
  }
}
