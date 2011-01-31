#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/Image.hh>
#include <iostream>
#include <Graphics2D/Segmentation.hh>
#include <vector>
#include <Graphics2D/StructureTensor.hh>
#include <cstdlib>

using namespace Graphics2D;
using namespace std;

int main(int argc, char *argv[]) {
  
  Filter *mean = Filter::CreateMean(3,3);
//  Filter *mean = NULL;
  
  int w = 1024;
  int h = 768;
  
  Image image, filtered;
  
  if (argc > 3) {
    w = atoi(argv[1]);
    h = atoi(argv[2]);
    cout << "generating images with " << w << "x" << h << " image size." << endl;
  } else {
    cout << "generating images with " << w << "x" << h << " image size." << endl;
    cout << "use \"" << argv[0] << " width height\" to generate different sizes." << endl;
  }
  
  int w2 = w/2;
  int h2 = h/2;
  
  int w4 = w/4;
  int h4 = h/4;

  int h6 = h/6;

  image.Init(w,h);
  image.FillZero();
  image.SetColorModel(ImageBase::cm_Grey);
  PrimitivePolygon poly;
  poly.SetColor(Color::white());
  
  vector<Coordinate> coo;
  
  // one line
  
  coo.resize(5);
  coo[0].Set(0,h);
  coo[1].Set(0,h-h2);
  coo[2].Set(w,h4);
  coo[3].Set(w,h);
  coo[4].Set(0,h);
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-one-line1.ppm");
  
  image.FillZero();

  coo.resize(5);
  coo[0].Set(0,0);
  coo[1].Set(w4,0);
  coo[2].Set(w-w2,h);
  coo[3].Set(0,h);
  coo[4].Set(0,0);
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-one-line2.ppm");
  
  image.FillZero();
  
  // two line
  
  coo.resize(4);
  coo[0].Set(0,0);
  coo[1].Set(w,h4);
  coo[2].Set(0,h2);
  coo[3].Set(0,0);
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-two-line1.ppm");
  
  image.FillZero();

  coo.resize(4);
  coo[0].Set(w2,0);
  coo[1].Set(w4,h);
  coo[2].Set(w-w4,h);
  coo[3].Set(w2,0);
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-two-line2.ppm");
  
  image.FillZero();

  // four line
  
  image.FillZero();

  coo.resize(5);
  coo[0].Set(0,h-h6);
  coo[1].Set(w,h4);
  coo[2].Set(w-w4,h);
  coo[3].Set(w4,0);
  coo[4].Set(0,h-h6);
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-four-line1.ppm");
  
  image.FillZero();

  coo.resize(6);
  coo[0].Set(0.5*w,0.5*h);
  coo[1].Set(0.1*w,0.9*h);
  coo[2].Set(0.3*w,0.2*h);
  coo[3].Set(0.9*w,0.3*h);
  coo[4].Set(0.6*w,0.8*h);
  coo[5].Set(0.5*w,0.5*h);
  
  poly.SetCoordinates(coo);
  poly.Draw(&image);
  
  filtered = image;
  if (mean) mean->FilterImage(image, filtered);
  filtered.SavePPM("test-hough-polygon.ppm");
  
  return 0;
}
