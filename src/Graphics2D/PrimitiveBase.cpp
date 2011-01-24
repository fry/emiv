#include <Graphics2D/PrimitiveBase.hh>

namespace Graphics2D {

  PrimitiveBase::PrimitiveBase(const Color &color) {
    color_ = color;
    coords_.clear();
    name_ = "Basetype";
  }
  
  PrimitiveBase::~PrimitiveBase() {
    // TODO Auto-generated destructor stub
  }
  
  void PrimitiveBase::SetColor(const Color &color) {
    color_ = color;
  }
  
  void PrimitiveBase::SetCoordinates(const std::vector<Coordinate> &coords) {
    if (coords_.size() != coords.size()) {
      std::cout << "Primitive " << name_ << " needs " << coords_.size() << " coordinates. But " << coords.size() << " coordinates given." << std::endl;
      return;
    } 
    coords_ = coords;
  }
  
  void PrimitiveBase::Rotate(float angle) {
    Coordinate mean;
    if (coords_.size() < 2) return;
    for (unsigned int i=0;i<coords_.size();i++) {
      mean = mean+coords_[i];
    }
    mean = mean*(1.0f/(float)coords_.size());
    for (unsigned int i=0;i<coords_.size();i++) {
      coords_[i].Rotate(mean, angle);
    }
  }
  
  void PrimitiveBase::GetCoordinates(std::vector<Coordinate> &coords) {
    coords = coords_;
  }
  
  void PrimitiveBase::GetName(std::string &name) {
    name = name_;
  }

  void PrimitiveBase::GetColor(Color &color) {
    color = color_;
  }

}
