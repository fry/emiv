#include <Graphics2DBase/PrimitiveBase.hh>

namespace Graphics2D {
  PrimitiveBase::PrimitiveBase(const Color& color): color_(color) {
  }
  
  void PrimitiveBase::SetCoordinates(const std::vector<Coordinate>& coordinates) {
    if (coordinates.size() != coordinates_.size())
      throw std::runtime_error("Invalid amount of coordinates");
    coordinates_ = coordinates;
  }
  
  void PrimitiveBase::GetName(std::string &name) {
    name = name_;
  }
}
