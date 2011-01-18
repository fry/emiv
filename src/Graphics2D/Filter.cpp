/**
 * Filter Factory for NxN filters. Non-generic filters are realized by static methods.
 * @author fkellner 12/10 
 */
#include <iostream>
#include <Graphics2D/Filter.hh>
#include <vector>
#include <algorithm>
#include <Graphics2D/ColorConversion.hh>
#include <cmath>

using namespace std;

namespace Graphics2D {

  Filter* Filter::CreateMean(int width, int height) {
    // filter mask is initialized with "ones"
    std::vector< std::vector<int> > mask(width, vector<int>(height, 1));
    // filter using this mask is returned
    return new Filter(mask);
  }

  Filter* Filter::CreateBinomial(int width) {
    // initialize with zeros
    std::vector< std::vector<int> > mask(width, vector<int>(width, 0));

    // first row
    for (int x=0;x<width;x++) {
      mask[x][0] = Binomial_(width-1,x);
    }
    // first column
    for (int y=0;y<width;y++) {
      mask[0][y] = Binomial_(width-1,y);
    }
    // rest is filled by outer product
    for (int x=1;x<width;x++) {
      for (int y=1;y<width;y++) {
        mask[x][y] = mask[x][0]*mask[0][y];
      }
    }
    // debug output
//    for (int x=0;x<width;x++) {
//      for (int y=0;y<width;y++) {
//        cout << mask[x][y] << " ";
//      }
//      cout << endl;
//    }
    return new Filter(mask);
  }

  Filter* Filter::CreateGradX() {
    // gradx is (-1 0 -1) / 2
    std::vector< std::vector<int> > mask(3, vector<int>(1,0));
    mask[0][0] = -1;
    mask[2][0] = 1;
    return new Filter(mask, 2);
  }

  Filter* Filter::CreateGradY() {
    // gradx is (-1 0 -1)^T / 2
    std::vector< std::vector<int> > mask(1, vector<int>(3,0));
    mask[0][0] = -1;
    mask[0][2] = 1;
    return new Filter(mask, 2);
  }

  Filter* Filter::CreateLaplace() {
    // laplace filter needs to be scaled with factor 8, because range is -1020 to 1020
    std::vector< std::vector<int> > mask(3, vector<int>(3,0));
    mask[0][1] = -1;
    mask[1][0] = -1;
    mask[1][1] = 4;
    mask[1][2] = -1;
    mask[2][1] = -1;
    return new Filter(mask, 8);
  }

  void Filter::MeanRecursive(const Image& src, Image &dst, int width, int height) {
    Image tmp;
    register int w = src.GetWidth();
    register int h = src.GetHeight();
    register int hmw = (width-1) / 2;
    register int hmh = (height-1) / 2;
    register int x,y;
    tmp.Init(h,w);
    tmp.FillZero();
    dst.Init(w,h);
    dst.FillZero();
    register unsigned char *srcdata = src.GetData();
    register unsigned char *tmpdata = tmp.GetData();
    register unsigned char *dstdata = dst.GetData();
    register unsigned char *curdata;
    register unsigned char *prev, *next;
    int sum[3];
    
    for (y=0;y<h;y++) {
      sum[2] = sum[1] = sum[0] = 0;
      curdata = srcdata + (y*w)*3;
      for (x=0;x<width;x++) {
        sum[0] += *curdata++;
        sum[1] += *curdata++;
        sum[2] += *curdata++;
      }
      curdata = tmpdata + (hmw*h+y)*3;
      *curdata++ = sum[0] / width;
      *curdata++ = sum[1] / width;
      *curdata = sum[2] / width;
      curdata = srcdata + y*w*3 + hmw*3;
      prev = curdata-hmw*3;
      next = curdata+(hmw+1)*3;
      for (x=hmw+1;x<w-hmw;x++) {
        curdata = tmpdata + (x*h+y)*3;
        sum[0] = sum[0] - *(prev++) + *(next++);
        sum[1] = sum[1] - *(prev++) + *(next++);
        sum[2] = sum[2] - *(prev++) + *(next++);
        *curdata++ = sum[0] / width;
        *curdata++ = sum[1] / width;
        *curdata   = sum[2] / width;
      }
    }

    for (y=0;y<w;y++) {
      sum[2] = sum[1] = sum[0] = 0;
      curdata = tmpdata + (y*h)*3;
      for (x=0;x<height;x++) {
        sum[0] += *curdata++;
        sum[1] += *curdata++;
        sum[2] += *curdata++;
      }
      curdata = dstdata + (hmh*w+y);
      *curdata++ = sum[0] / width;
      *curdata++ = sum[1] / width;
      *curdata   = sum[2] / width;
      curdata = tmpdata + y*h*3 + hmw*3;
      prev = curdata-hmh*3;
      next = curdata+(hmh+1)*3;
      for (x=hmh+1;x<h-hmh;x++) {
        curdata = dstdata + (x*w+y)*3; 
        sum[0] = sum[0] - *(prev++) + *(next++);
        sum[1] = sum[1] - *(prev++) + *(next++);
        sum[2] = sum[2] - *(prev++) + *(next++);
        *curdata++ = sum[0] / width;
        *curdata++ = sum[1] / width;
        *curdata   = sum[2] / width;
      }
    }
    
    dst.SetColorModel(src.GetColorModel());
  }

  void Filter::GradMag(const Image& src, Image &dst) {
    Filter *gx = CreateGradX();
    Filter *gy = CreateGradY();
    Image gxImage, gyImage;
    int w=src.GetWidth();
    int h=src.GetHeight();
    // get gradients in x and y dir
    gx->FilterImage(src,gxImage);
    gy->FilterImage(src,gyImage);
    dst.Init(w,h);
    unsigned char *gxdata = gxImage.GetData();
    unsigned char *gydata = gyImage.GetData();
    unsigned char *dstdata = dst.GetData();
    int val;
    // calculate magnitude
    float scale = 255.0f/sqrt(128*128+128*128);
    for (int x=0;x<w*h*3;x+=3) {
      int gx = (int)gxdata[x]-127;
      int gy = (int)gydata[x]-127;
      val = sqrt(gx*gx+gy*gy)*scale;
      dstdata[x+0]=(unsigned char)val;
      dstdata[x+1]=dstdata[x+0];
      dstdata[x+2]=dstdata[x+1];
    }
    // dont forget to delete the filters
    delete gx;
    delete gy;
    dst.SetColorModel(ImageBase::cm_Grey);
  }

  void Filter::Rank3x3(const Image& src, Image &dst, int rank) {
    Image grey;
    if (src.GetColorModel() == ImageBase::cm_Grey) {
      grey = src;
    } else {
      ColorConversion::ToGrey(src,grey);
    }
    if (rank < 0 || rank > 8) { 
      cout << "rank must be in 0..8, using rank = 4" << endl;
      rank = 4;
    }
    int w=src.GetWidth();
    int h=src.GetHeight();
    dst.Init(w,h);
    unsigned char *srcdata = grey.GetData();
    unsigned char *dstdata = dst.GetData();

    vector<int> window(9);
    for (int y=1;y<h-1;y++) {
      for (int x=1;x<w-1;x++) {
        int idx = 0;
        // collect values of current window
        for (int yw=y-1;yw<=y+1;yw++) {
          for (int xw=x-1;xw<=x+1;xw++) {
            window[idx++] = srcdata[(yw*w+xw)*3];
          }
        }
        // sort the values by ascending order
        std::sort(window.begin(), window.end());
        // return value of desired rank
        dstdata[(y*w+x)*3+0] = window[rank];
        dstdata[(y*w+x)*3+1] = window[rank];
        dstdata[(y*w+x)*3+2] = window[rank];
      }
    }
  }

  Filter::Filter(const std::vector< std::vector<int> > &mask, int scale) {
    mask_ = mask;
    width_ = mask_.size();
    height_ = mask_[0].size();
    if (width_ % 2 != 1 || height_ % 2 != 1) {
      std::cout << "Filter must have odd size, " << width_ << " " << height_ << std::endl;
    }
    sum_ = 0;
    for (unsigned int x=0;x<width_;x++) {
      for (unsigned int y=0;y<height_;y++) {
        sum_ += mask_[x][y];
      }
    }
    hmw_ = (width_-1) / 2;
    hmh_ = (height_-1) / 2;

    scale_ = scale;
    offset_ = 255*(scale_/2);
  }

  Filter::~Filter() {
  }

  void Filter::FilterImage(const Image& src, Image &dst) {
    int w = src.GetWidth();
    int h = src.GetHeight();
    
    dst.Init(w,h);
    dst.SetColorModel(src.GetColorModel());
    
    register unsigned char *srcdata = src.GetData();
    register unsigned char *dstdata = dst.GetData();
    
    register unsigned char *curdata;
    
    // special case for grey images
    if (src.GetColorModel() == ImageBase::cm_Grey) {
      register int idx, val;
      register int nextLineOffset = w*3-3*width_;
      register int windowOffset = -hmh_*w*3 -hmw_*3;
      register unsigned char result;
      for (int y=hmh_;y<h-hmh_;y++) {
        for (int x=hmw_;x<w-hmw_;x++) {
          val = 0;
          idx = (y*w+x)*3;
          curdata = srcdata + idx + windowOffset;
          for (unsigned int yw=0;yw<height_;yw++) {
            for (unsigned int xw=0;xw<width_;xw++) {
              val += *curdata * mask_[xw][yw];
              curdata+=3;
            }
            curdata+=nextLineOffset;
          }
          curdata = dstdata+idx;
          if (sum_ == 0) {
            result = (val + offset_) / scale_;
          } else {
            result = val / sum_;
          }
          *curdata++ = result;
          *curdata++ = result;
          *curdata   = result;
        }
      }
    } else {
      register int idx, weight;
      int  val[3];
      register int nextLineOffset = w*3-3*width_;
      register int windowOffset = -hmh_*w*3 -hmw_*3;
      for (int y=hmh_;y<h-hmh_;y++) {
        for (int x=hmw_;x<w-hmw_;x++) {
          // stores weighted sums for each channel
          val[0] = 0;
          val[1] = 0;
          val[2] = 0;
          // x and y in filter mask
          // index of pixel in data array
          idx = (y*w+x)*3;
          // inner loops walk the window
          curdata = srcdata + idx + windowOffset;
          for (unsigned int yw=0;yw<height_;yw++) {
            for (unsigned int xw=0;xw<width_;xw++) {
              // calculate weighted sums for each channel individually
              weight = mask_[xw][yw];
              val[0] += *curdata++ * weight;
              val[1] += *curdata++ * weight;
              val[2] += *curdata++ * weight;
            }
            curdata+=nextLineOffset;
          }
          curdata = dstdata + idx;
          if (sum_ == 0) {
            *curdata++ = (val[0] + offset_) / scale_;
            *curdata++ = (val[1] + offset_) / scale_;
            *curdata   = (val[2] + offset_) / scale_;
          } else {
            *curdata++ = val[0] / sum_;
            *curdata++ = val[1] / sum_;
            *curdata   = val[2] / sum_;
          }
        }
      }
    }
  }

  int Filter::Binomial_(int n, int k) {
    if (k==0 || k==n) return 1;
    return Binomial_(n-1,k-1)+(Binomial_(n-1, k));
  }

  
  
  void Filter::FilterImage(const Image& src, FloatImage &dst) {
    if (!src.Valid() || src.GetColorModel() != ImageBase::cm_Grey) {
      cout << "FilterImage to float only accepts grey images" << endl;
      return;
    }

    int w = src.GetWidth();
    int h = src.GetHeight();
    
    dst.Init(w,h);
    
    register unsigned char *srcdata = src.GetData();
    register float *dstdata = dst.GetData();
    
    register unsigned char *curdata;
    
    register int idx;
    float val;
    register int nextLineOffset = w*3-3*width_;
    register int windowOffset = -hmh_*w*3 -hmw_*3;
    for (int y=hmh_;y<h-hmh_;y++) {
      for (int x=hmw_;x<w-hmw_;x++) {
        val = 0.0f;
        idx = (y*w+x)*3;
        curdata = srcdata + idx + windowOffset;
        for (unsigned int yw=0;yw<height_;yw++) {
          for (unsigned int xw=0;xw<width_;xw++) {
            val += (*curdata * mask_[xw][yw])/ 255.0f;
            curdata+=3;
          }
          curdata+=nextLineOffset;
        }
        if (sum_ == 0) {
          dstdata[y*w+x] = (float)val / (float)scale_; 
        } else {
          dstdata[y*w+x] = (float)val / (float)sum_; 
        }
      }
    }
  }

  void Filter::FilterImage(const FloatImage& src, FloatImage &dst) {
    if (!src.Valid()) {
      cout << "FilterImage not valid" << endl;
      return;
    }

    int w = src.GetWidth();
    int h = src.GetHeight();
    
    dst.Init(w,h);
    dst.FillZero();
    
    register float *srcdata = src.GetData();
    register float *dstdata = dst.GetData();
    
    register float *curdata;
    
    register int idx;
    float val;
    register int nextLineOffset = w-width_;
    register int windowOffset = -hmh_*w -hmw_;

    for (int y=hmh_;y<h-hmh_;y++) {
      for (int x=hmw_;x<w-hmw_;x++) {
        val = 0.0f;
        idx = (y*w+x);
        curdata = srcdata + idx + windowOffset;
        for (unsigned int yw=0;yw<height_;yw++) {
          for (unsigned int xw=0;xw<width_;xw++) {
            val += *curdata * (float)(mask_[xw][yw]);
            curdata++;
          }
          curdata+=nextLineOffset;
        }
        curdata = dstdata + idx;
        if (sum_ == 0) {
          *curdata = val / (float)scale_; 
        } else {
          *curdata = val / (float)sum_; 
        }
      }
    }
  }

}
