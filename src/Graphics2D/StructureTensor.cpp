#include <Graphics2D/StructureTensor.hh>

using namespace Graphics2D;

StructureTensor::StructureTensor(int filterSize) {
  filterGx_ = Filter::CreateGradX();
  filterGy_ = Filter::CreateGradY();
  
  filterBinomial_ = Filter::CreateBinomial(filterSize);
  
}

StructureTensor::~StructureTensor() {
  delete filterGx_;
  delete filterGy_;
  delete filterBinomial_;
}

void StructureTensor::SetFromImage(const Image &input) {
  const int w = input.GetWidth();
  const int h = input.GetHeight();
  gxx_.Init(w, h);
  gxy_.Init(w, h);
  gyy_.Init(w, h);
  
  filterGx_->FilterImage(input, gx_);
  filterGy_->FilterImage(input, gy_);
  
  for (int x = 0; x < input.GetWidth(); x++) {
    for (int y = 0; y < input.GetHeight(); y++) {
      const float gx = gx_.GetPixel(x, y);
      const float gy = gy_.GetPixel(x, y);
      gxx_.SetPixel(x, y, gx * gx);
      gxy_.SetPixel(x, y, gx * gy);
      gyy_.SetPixel(x, y, gy * gy);
    }
  }
  
  filterBinomial_->FilterImage(gxx_, jxx_);
  filterBinomial_->FilterImage(gxy_, jxy_);
  filterBinomial_->FilterImage(gyy_, jyy_);
}

void StructureTensor::FoerstnerDetector(float thres, Image &corners) {
  const int w = jxx_.GetWidth();
  const int h = jxx_.GetHeight();
  
  corners.Init(w, h);
  corners.SetColorModel(ImageBase::cm_RGB);
  corners.FillZero();
  
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y++) {
      const float jxx = jxx_.GetPixel(x, y);
      const float jyy = jyy_.GetPixel(x, y);
      const float jxy = jxy_.GetPixel(x, y);
      const float det = jxx * jyy - jxy * jxy;
      const float trace = jxx + jyy;
      if (trace <= thres) {
        corners.SetPixel(x, y, 0, 0);
        corners.SetPixel(x, y, 1, 0);
        corners.SetPixel(x, y, 2, 0);
      } else {
        const float q = (4 * det) / (trace * trace);
        if (q <= 0.5) {
          corners.SetPixel(x, y, 1, 255);
        } else {
          corners.SetPixel(x, y, 0, 255);
        }
      }
    }
  }
}

void StructureTensor::HarrisCornerDetector(float thres, Image &corners) {
  const int w = jxx_.GetWidth();
  const int h = jxx_.GetHeight();
  
  FloatImage harris;
  harris.Init(w, h);
  
  corners.Init(w, h);
  corners.SetColorModel(ImageBase::cm_Grey);
  
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y++) {
      const float jxx = jxx_.GetPixel(x, y);
      const float jyy = jyy_.GetPixel(x, y);
      const float jxy = jxy_.GetPixel(x, y);
      const float det = jxx * jyy - jxy * jxy;
      const float trace = jxx + jyy;
      const float ch = det - 0.04 * trace * trace;
      harris.SetPixel(x, y, ch);
    }
  }
  
  for (int y = 0; y < h; y++) {
    float max = 0.0;
    for (int x = 0; x < w; x++) {
      const float ch = harris.GetPixel(x, y);
      if (ch > thres) {
        if (ch >= max) {
          max = ch;
        } else if (x > 0) {
          corners.SetPixel(x - 1, y, 2, 255);
          max = 0.0;
        }
      }
    }
  }
  

    
  for (int x = 0; x < w; x++) {
    float max = 0.0;
    for (int y = 0; y < h; y++) {
      const float ch = harris.GetPixel(x, y);
      if (ch > thres) {
        if (ch >= max) {
          max = ch;
          if (y > 0)
            corners.SetPixel(x, y - 1, 2, 0);
        } else if (y > 0) {
          if (corners.GetPixel(x, y - 1, 2) == 255)
            corners.SetPixel(x, y - 1, 2, 255);
          max = 0.0;
        }
      }
    }
  }
}