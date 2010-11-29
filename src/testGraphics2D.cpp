// Gruppe 8
// Serie 2
// Alexander Koch, 773036
// Eike Siewertsen, 847522

#include <Graphics2D/Image.hh>
#include <iostream>
#include <Graphics2DBase/Canvas2D.hh>
#include <Graphics2D/Painter.hh>
#include <Graphics2D/PrimitiveBox.hh>
#include <Graphics2DBase/Coordinate.hh>
#include <vector>

using namespace Graphics2D;

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cout << "usage: " << argv[0] << " background.ppm" << std::endl;
    return 0;
  }
  
  Image image;
  if (argc >= 2)
    image.LoadPPM(argv[1]);
  
  Image image_surface;
  image_surface.Init(image.GetWidth(), image.GetHeight());
  
  Canvas2D c2d;
  c2d.Init(100, 100, image.GetWidth(), image.GetHeight());
  
  Painter painter(image);
  c2d.RegisterImage(&image_surface);
  c2d.RegisterPainter(&painter);

  c2d.Start();
  
  /*std::vector<Coordinate> coords;
  coords.push_back(Coordinate(10, 10));
  coords.push_back(Coordinate(50, 30));
  coords.push_back(Coordinate(20, 50));
  PrimitivePolygon p(Color::black(), coords);
  p.Draw(&image);*/
  
  return 0;
}
