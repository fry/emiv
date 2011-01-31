
#include <Graphics2D/HoughTransform.hh>
#include <Graphics2D/Filter.hh>

using namespace std;

namespace Graphics2D {

  HoughTransform::HoughTransform() {
    // TODO Auto-generated constructor stub

  }

  HoughTransform::~HoughTransform() {
    // TODO Auto-generated destructor stub
  }

  
  void HoughTransform::StandardHoughTransform(const Image &input, const int resolution, vector<PrimitiveLine> &lines) {
    Create2DHistogram_(input, resolution);
    Filter::NonMaximumSuppression(houghspace_, houghspace_, 100, 21);
    Image test;
    houghspace_.GetAsGreyImage(test);
    test.SavePPM("houghsimplemax.ppm");
    GetLines_(input.GetWidth(), input.GetHeight(), lines);
  }

  void HoughTransform::FastHoughTransform(const StructureTensor &input, const int resolution, vector<PrimitiveLine> &lines) {
    Create2DHistogramFromStructureTensor_(input, resolution);
    Filter::NonMaximumSuppression(houghspace_, houghspace_, 10, 21);
    Image test;
    houghspace_.GetAsGreyImage(test);
    test.SavePPM("houghfastmax.ppm");
    GetLines_(input.GetWidth(), input.GetHeight(), lines);
  }

  // ---- protected
  
  void HoughTransform::Create2DHistogram_(const Image &input, const int resolution) {
    int w = (int)input.GetWidth();
    int h = (int)input.GetHeight();
    
    int angleBins = 180 * resolution;
    int maxDistance = (int)(ceil(sqrt(w*w+h*h)))/2;
    
    float binToRadians = M_PI / (180.0f * resolution);
    
    houghspace_.Init(angleBins, maxDistance*2);
    houghspace_.FillZero();
    
    Image magnitude;
    magnitude.Init(w,h);
    magnitude.FillZero();
    Filter::GradMag(input, magnitude);
    magnitude.SavePPM("gradmag.ppm");
    
    unsigned char *magdata = magnitude.GetData();
    
    int xoffs = w/2;
    int yoffs = h/2;

    for (int y=1;y<h-1;y++) {
      for (int x=1;x<w-1;x++) {
        
        int idx = y*w+x;

        float mag = magdata[idx*3];

        if (mag > 100) {
          for (int i=0;i<angleBins;i++) {
            
            float phi = (float)(i)*binToRadians;
            
            float d = (x-xoffs)*cos(phi)+(y-yoffs)*sin(phi) + maxDistance;
            houghspace_.SetPixel(i, (int)d, houghspace_.GetPixel(i,(int)d)+1);
          }
        }
      }
    }
    
    Image test;
    houghspace_.GetAsGreyImage(test);
    test.SavePPM("houghsimple.ppm");
    
  }
  
  void HoughTransform::Create2DHistogramFromStructureTensor_(const StructureTensor &input, const int resolution) {
    
    FloatImage Jxx, Jyy, Jxy;
    input.GetStructureTensor(Jxx, Jxy, Jyy);
    
    int w = (int)input.GetWidth();
    int h = (int)input.GetHeight();
    
    int angleBins = 180 * resolution;
    int maxDistance = (int)(ceil(sqrt(w*w+h*h)))/2;
    
    float radiansTobin = (180.0f * resolution) / M_PI;
    
    houghspace_.Init(angleBins, maxDistance*2);
    houghspace_.FillZero();

    float *gxxdata = Jxx.GetData();
    float *gxydata = Jxy.GetData();
    float *gyydata = Jyy.GetData();

    int xoffs = w/2;
    int yoffs = h/2;

    float meantrace=0.0f;
    for (int y=0;y<h;y++) {
      for (int x=0;x<w;x++) {
        int idx = y*w+x;
        meantrace+=gxxdata[idx]+gyydata[idx];
      }
    }
    meantrace/=(float)(w*h);
    
    for (int y=0;y<h;y++) {
      for (int x=0;x<w;x++) {
        
        int idx = y*w+x;

        if (gxxdata[idx]+gyydata[idx] > meantrace) {
          
          float phi = atan(2*gxydata[idx]/(gyydata[idx]-gxxdata[idx]))/2.0;
  
          if (gxxdata[idx]-gyydata[idx]>=0) { 
            phi= -phi+M_PI;
          } else {
            phi = -phi+M_PI/2;
          }
          if (phi>M_PI) phi-=M_PI;
          if (phi<0) phi+=M_PI;
          
          float d = (x-xoffs)*cos(phi)+(y-yoffs)*sin(phi) + maxDistance;
          int i = (int)rint(phi*radiansTobin);
          
          houghspace_.SetPixel(i, (int)d, houghspace_.GetPixel(i,(int)d)+1);
        }
      }
    }
    
    Image test;
    houghspace_.GetAsGreyImage(test);
    test.SavePPM("houghfast.ppm");

  }

  
  void HoughTransform::GetLines_(int imWidth, int imHeight, std::vector<PrimitiveLine> &lines) {
    lines.clear();
    
    int angleBins = (int)houghspace_.GetWidth();
    int maxDistance = (int)houghspace_.GetHeight()/2;
    
    int resolution = angleBins / 180;
    
    float binToRadians = M_PI / (180.0f * resolution);
    
    int xoffs = imWidth/2;
    int yoffs = imHeight/2;


    for (int y=0;y<maxDistance*2;y++) {
      for (int x=0;x<angleBins;x++) {
        if (houghspace_.GetPixel(x,y)>50) {
          float angle = float(x)*binToRadians-M_PI/2;
          if (angle>-M_PI/4 && angle<M_PI/4) {
            float distance = float(y) - maxDistance;
//            cout << angle*180.0f/M_PI<< "  - blub1 " << angle << " - weight:" << houghspace_.GetPixel(x,y) << endl;
            vector<Coordinate> coords(2);
            PrimitiveLine pl;
            pl.SetColor(Color::blue());
            
            double m = tan(angle);
            double b = (distance) / cos(angle) + yoffs;

            cout << "mx: " << m << " b " << b << endl;
            coords[0] = Coordinate(0,-xoffs*m+b);
            coords[1] = Coordinate(imWidth, xoffs*m+b);
            pl.SetCoordinates(coords);
            lines.push_back(pl);
          } else {
            float distance = float(y) - maxDistance;
//            cout << angle*180.0f/M_PI<< "  - blub2 " << angle << " - weight:" << houghspace_.GetPixel(x,y) << endl;
            vector<Coordinate> coords(2);
            PrimitiveLine pl;
            pl.SetColor(Color::yellow());
            
            double m = -tan(angle+M_PI/2);
            double b = (distance) / cos(angle+M_PI/2) + xoffs;
            cout << "my: " << m << " b " << b << "  distance:" << distance << endl;

            coords[0] = Coordinate(-yoffs*m+b,0);
            coords[1] = Coordinate(yoffs*m+b,imHeight);
            pl.SetCoordinates(coords);
            lines.push_back(pl);

          }
        }
      }
    }
  }
}
