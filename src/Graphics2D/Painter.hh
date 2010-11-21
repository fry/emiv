#ifndef PAINTER_HH
#define PAINTER_HH

#include <memory>

#include <Graphics2DBase/PrimitiveBase.hh>
#include <Graphics2DBase/PainterBase.hh>

namespace Graphics2D {
  class Painter: public PainterBase {
    public:
      Painter(ImageBase* background);
      virtual ~Painter();
      
      // Add a primitive to this Painter, takes ownership of the pointer
      void AddPrimitive(PrimitiveBase* primitive);
      
      virtual void Draw();
      
      virtual std::string GetColorString();
      virtual std::string GetString();
      
      virtual void MouseDown(int x, int y);
      
      virtual void MouseUp(int x, int y);
      
      virtual void MouseMove(int x, int y);
      
      virtual void KeyPressed(unsigned char ch, int x, int y);
    protected:
      Color GetColor();
      PrimitiveBase* GetCurrentPrimitive(int x, int y);
      
      // The background image to use
      ImageBase* background_;
      
      // The ghost primitive shown when drawing a primitive
      std::auto_ptr<PrimitiveBase> temporary_primitive_;
      
      // The list of primitives contained in this picture
      std::vector<PrimitiveBase*> primitives_;
      
      // The points of a polygon that is being drawn
      std::vector<Coordinate> polygon_points_;
      
      std::string color_string_;
      std::string primitive_string_;
      
      int draw_start_x, draw_start_y;
  };
}

#endif
