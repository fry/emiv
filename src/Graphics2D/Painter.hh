/*
 * painter is a container for primitives, new primitives are added my gui interaction
 */
#ifndef PAINTER_HH_
#define PAINTER_HH_

#include <vector>
#include <Graphics2DBase/PainterBase.hh>
#include <Graphics2D/PrimitiveBase.hh>
#include <Graphics2D/Color.hh>

namespace Graphics2D {

  class Painter : public PainterBase {
    public:
      Painter();
      virtual ~Painter();
      
      void AddPrimitive(PrimitiveBase *primitive);
      
      virtual std::string GetColorString();
      virtual std::string GetString();
      
      virtual void Draw();
      
      virtual void MouseDown(int x, int y);
      
      virtual void MouseUp(int x, int y);
      
      virtual void MouseMove(int x, int y);

      virtual void KeyPressed(unsigned char ch, int x, int y);
      
    private:
      enum DrawMode {
        point=0,
        line,
        box,
        polygon,
        star
      };
      std::vector<PrimitiveBase*> primitives_;
      std::vector<Coordinate> coo_;

      DrawMode drawMode_;
      Color color_;
      
      PrimitiveBase* previewPrimitive_;
      
      bool togrey_;
      
      bool enhance_;
      
      bool drawHisto_;
  };

}

#endif /* PAINTER_HH_ */
