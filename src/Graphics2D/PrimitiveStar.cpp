#include <Graphics2D/PrimitiveStar.hh>

namespace Graphics2D {

  PrimitiveStar::PrimitiveStar(int numCones) {
    color_ = Color::black();
    name_ = "Star";
    coords_.clear();
    numCones_ = numCones;
    closed_ = true;
  }
    
  PrimitiveStar::~PrimitiveStar() {
    // TODO Auto-generated destructor stub
  }

  void PrimitiveStar::SetCoordinates(const std::vector<Coordinate> &coords) {
    
    if (coords.size() != 2) {
      std::cout << "A star has one coordinate for center and a second one determining the size" << std::endl;
      return;
    }
    
    coords_.clear();
    Coordinate middle = coords[0];
    
    float radius = (float)middle.Dist(coords[1]);
    
    Coordinate rotlarge = middle + Coordinate(0.0f,1.0f) * radius;
    Coordinate rotsmall = middle + Coordinate(0.0f,1.0f) * (radius/2);
    
    float rotangle = 2.0f * M_PI / (float)numCones_;
    
    rotsmall.Rotate(middle, rotangle / 2.0f);
    
    for (int i=0;i<numCones_;i++) {
      coords_.push_back(rotlarge);
      coords_.push_back(rotsmall);
      rotlarge.Rotate(middle, rotangle );
      rotsmall.Rotate(middle, rotangle );
    }
    coords_.push_back(rotlarge);
    rotlarge.Rotate(middle, rotangle );
    PrimitivePolygon::SetCoordinates(coords_);
  }
}
