#ifndef PRIMITIVEBASE_HH
#define PRIMITIVEBASE_HH

#include <Graphics2DBase/Color.hh>
#include <Graphics2DBase/Coordinate.hh>
#include <Graphics2DBase/ImageBase.hh>
#include <vector>
#include <stdexcept>

namespace Graphics2D {
  class PrimitiveBase {
  public: 
    PrimitiveBase(const Color& color);
    virtual ~PrimitiveBase() {};
    
    inline const std::vector<Coordinate>& GetCoordinates() const {
      return coordinates_;
    }
    
    virtual void SetCoordinates(const std::vector<Coordinate>& coordinates);
    
    void GetName(std::string &name);
    
    inline const Color& GetColor() const {
      return color_;
    }
    
    inline void SetColor(const Color& color) {
      color_ = color;
    }
   
    virtual void Draw(ImageBase *im) = 0;
    
    void Rotate(float angle);
  protected:
    std::vector<Coordinate> coordinates_;
    Color color_;
    std::string name_;
  };
}

#endif
