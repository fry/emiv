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
  
  void PrimitiveBase::Rotate(float angle) {
    if (coordinates_.empty())
      return;

    Coordinate origin;
    
    std::vector<Coordinate>::iterator iter, end;
    end = coordinates_.end();
    
    // Determine arithmetic mean
    for (iter = coordinates_.begin(); iter != end; ++iter) {
      origin += *iter;
    }
    
    origin *= 1.0f / coordinates_.size();

    // Call rotate on every point
    for (iter = coordinates_.begin(); iter != end; ++iter) {
      iter->Rotate(origin, angle);
    }
  }
}
