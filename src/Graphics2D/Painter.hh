#ifndef PAINTER_HH
#define PAINTER_HH

#include <memory>

#include <Graphics2DBase/PrimitiveBase.hh>
#include <Graphics2DBase/PainterBase.hh>
#include <Graphics2D/Image.hh>
#include <Graphics2D/Histogram.hh>

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
      void UpdateHistogram();
      
      // The background image specified by the user
      ImageBase* specified_background_;
      
      // The background image to use
      Image background_;
      
      // The histogram of the background image
      Histogram hist_red_;
      Histogram hist_green_;
      Histogram hist_blue_;
      
      Histogram hist_hue_;
      Histogram hist_saturation_;
      Histogram hist_value_;
      
      // Whether to display the histogram data
      bool display_histogram_;
      
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
