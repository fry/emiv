#include <Graphics2D/StructureTensor.hh>
#include <iostream>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/Color.hh>
#include <Graphics2D/Coordinate.hh>
#include <Graphics2D/PrimitiveLine.hh>

using namespace std;

namespace Graphics2D {

  StructureTensor::StructureTensor(int filterSize) {
    filterGx_ = Filter::CreateGradX();
    filterGy_ = Filter::CreateGradY();
    filterBinomial_ = Filter::CreateBinomial(filterSize);
    halfwinSize_ = filterSize/2;
  }

  StructureTensor::~StructureTensor() {
    delete filterGx_;
    delete filterGy_;
    delete filterBinomial_;
  }

  
  void StructureTensor::SetFromImage(const Image &input) {
    
    if (!input.Valid() || input.GetColorModel() != ImageBase::cm_Grey) {
      cout << "StructureTensor accepts grey images." << endl;
      return;
    }
    
    int w = (int)input.GetWidth();
    int h = (int)input.GetHeight();
    FloatImage tmpxx, tmpxy, tmpyy;
    tmpxy.Init(w,h);

    filterGx_->FilterImage(input, tmpxx);
    filterGy_->FilterImage(input, tmpyy);

    float *gxxdata = tmpxx.GetData();
    float *gxydata = tmpxy.GetData();
    float *gyydata = tmpyy.GetData();
    
    for (int i=0;i<w*h;i++) {
      gxydata[i] = gxxdata[i] * gyydata[i];
      gxxdata[i] = gxxdata[i] * gxxdata[i];
      gyydata[i] = gyydata[i] * gyydata[i];
    }

    filterBinomial_->FilterImage(tmpxx, gxx_);
    filterBinomial_->FilterImage(tmpxy, gxy_);
    filterBinomial_->FilterImage(tmpyy, gyy_);
    
  }
  
  void StructureTensor::FoerstnerDetector(float thres, Image &corners) {
    int w = (int)gxx_.GetWidth();
    int h = (int)gxx_.GetHeight();
    corners.Init(w, h);
    PrimitivePoint corner;
    corner.SetColor(Color::red());
    PrimitivePoint edge;
    edge.SetColor(Color::green());

    float *gxxdata = gxx_.GetData();
    float *gxydata = gxy_.GetData();
    float *gyydata = gyy_.GetData();

    for (int y=halfwinSize_;y<h-halfwinSize_;y++) {
      for (int x=halfwinSize_;x<w-halfwinSize_;x++) {

        int idx = y*w+x;
        
        float trace = gxxdata[idx]+gyydata[idx];
        float det = gxxdata[idx]*gyydata[idx] - gxydata[idx]*gxydata[idx];

        if (trace != 0) cout << trace << endl;
        if (trace > thres) {
          float q = 4*det / (trace*trace);
          if (q < 0.5) {
            edge.SetCoordinate(Coordinate(x,y));
            edge.Draw(&corners);
          } else {
            corner.SetCoordinate(Coordinate(x,y));
            corner.Draw(&corners);
            cout << endl << gxxdata[idx] << " " << gxydata[idx] << " " << gyydata[idx] << endl; 
          }
        }
      }
    }
  }
  
  void StructureTensor::HarrisCornerDetector(std::vector<Coordinate>& corners) {
    int w = (int)gxx_.GetWidth();
    int h = (int)gxx_.GetHeight();

    PrimitivePoint corner;
    corner.SetColor(Color::blue());

    float *gxxdata = gxx_.GetData();
    float *gxydata = gxy_.GetData();
    float *gyydata = gyy_.GetData();
    
    FloatImage v, vmax;
    v.Init(w,h);
    v.FillZero();
    vmax.Init(w,h);
    vmax.FillZero();
    float *vdata = v.GetData();

    for (int y=halfwinSize_;y<h-halfwinSize_;y++) {
      for (int x=halfwinSize_;x<w-halfwinSize_;x++) {

        int idx = y*w+x;
        
        float trace = gxxdata[idx]+gyydata[idx];
        float det = gxxdata[idx]*gyydata[idx] - gxydata[idx]*gxydata[idx];

        vdata[idx] = det - 0.04 * trace * trace;
      }
    }
    Image strength;
    v.GetAsGreyImage(strength);
    
    Filter::NonMaximumSuppression(v, vmax, 0.0000000005);
    
    for (int y=0;y<h;y++) {
      for (int x=0;x<w;x++) {
        if (vmax.GetPixel(x,y)>0) {
          corners.push_back(Coordinate(x, y));
          corner.SetCoordinate(Coordinate(x,y));
          corner.Draw(&strength);
        }
      }
    }
    
    strength.SavePPM("harris-strength.ppm");
  }
}
