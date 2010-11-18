#ifndef PAINTER_HH
#define PAINTER_HH

#include <memory>

#include <Graphics2DBase/PrimitiveBase.hh>
#include <Graphics2DBase/PainterBase.hh>

namespace Graphics2D {
  class Painter: public PainterBase {
    public:
      Painter();
      virtual ~Painter();
      
      void AddPrimitive(PrimitiveBase* primitive);
      Color GetColor();
      
      virtual void Draw();
      
      virtual std::string GetColorString();
      virtual std::string GetString();
      
      virtual void MouseDown(int x, int y);
      
      virtual void MouseUp(int x, int y);
      
      virtual void MouseMove(int x, int y);
      
      virtual void KeyPressed(unsigned char ch, int x, int y);
    protected:
      std::auto_ptr<PrimitiveBase> temporary_primitive_;
      std::vector<PrimitiveBase*> primitives_;
      
      std::string color_string_;
      std::string primitive_string_;
      
      int draw_start_x, draw_start_y;
  };
}

#endif
