/*
 * Each Primitive implements this interface
 * @author fkellner, 10/10
 */

#ifndef PRIMITIVEBASE_H_
#define PRIMITIVEBASE_H_

#include <vector>
#include <Graphics2DBase/ImageBase.hh>
#include <Graphics2D/Color.hh>
#include <Graphics2D/Coordinate.hh>

namespace Graphics2D {

  class PrimitiveBase {
    public:
      // base constructor with default color
      PrimitiveBase(const Color &color = Color::black());
      virtual ~PrimitiveBase();
      
      // change color
      virtual void SetColor(const Color &color);
      
      // a primitive can have any number of coordinates
      virtual void SetCoordinates(const std::vector<Coordinate> &coords);

      // rotate coordinates around center of mass with angle in rad
      virtual void Rotate(float angle);
      
      void GetCoordinates(std::vector<Coordinate> &coords);
      
      // a name, defaults to "Base"
      void GetName(std::string &name);
      // get the color
      void GetColor(Color &color);
      
      // each primitive must implement it's own Draw function
      virtual void Draw(ImageBase *im) = 0;
      
    protected:
      
      Color color_;
      std::vector<Coordinate> coords_;
      
      std::string name_;
  };

}

#endif /* PRIMITIVEBASE_H_ */
