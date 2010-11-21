#include <Graphics2D/Painter.hh>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitiveLine.hh>
#include <Graphics2D/PrimitiveBox.hh>
#include <Graphics2D/PrimitivePolygon.hh>

namespace Graphics2D {
  Painter::Painter() {
  }
  
  Painter::~Painter() {
    // Delete all primitives we own
    std::vector<PrimitiveBase*>::iterator iter, end;
    end = primitives_.end();
    
    for (iter = primitives_.begin(); iter != end; ++iter) {
      delete *iter;
    }
    
    primitives_.clear();
  }
  
  void Painter::AddPrimitive(PrimitiveBase* primitive) {
    primitives_.push_back(primitive);
  }
  
  Color Painter::GetColor() {
    // Determine what color to use from the color string
    const std::string color = GetColorString();
    if (color == "White")
      return Color::white();
    else if (color == "Red")
      return Color::red();
    else if (color == "Green")
      return Color::green();
    else if (color == "Blue")
      return Color::blue();
      
    return Color::black();
  }
  
  PrimitiveBase* Painter::GetCurrentPrimitive(int x, int y) {
    if (primitive_string_ == "Point") {
      return new PrimitivePoint(GetColor(), Coordinate(x, y));
    } else if (primitive_string_ == "Line") {
      return new PrimitiveLine(GetColor(), Coordinate(draw_start_x, draw_start_y), Coordinate(x, y));
    } else if (primitive_string_ == "Box") {
      std::vector<Coordinate> boxCoords;
      boxCoords.push_back(Coordinate(draw_start_x,draw_start_y));
      boxCoords.push_back(Coordinate(x,y));
      return new PrimitiveBox(GetColor(), boxCoords);
    }
    
    return NULL;
  }
  
  void Painter::Draw() {
    // Clear the image
    image_->FillZero();
    
    // Iterate through contained primitives and draw them
    std::vector<PrimitiveBase*>::iterator iter, end;
    end = primitives_.end();
    
    for (iter = primitives_.begin(); iter != end; ++iter) {
      (*iter)->Draw(image_);
    }
    
    // If a "ghost primitive" is set, draw it
    if (temporary_primitive_.get() != 0)
      temporary_primitive_->Draw(image_);
  }

  std::string Painter::GetColorString() {
    return color_string_;
  }
  
  std::string Painter::GetString() {
    return primitive_string_;
  }
  
  void Painter::MouseDown(int x, int y) {
    draw_start_x = x;
    draw_start_y = y;
  }
  
  void Painter::MouseUp(int x, int y) {
    // Add the finished primitive
    PrimitiveBase* prim = GetCurrentPrimitive(x, y);
    AddPrimitive(prim);
    
    // And get rid of any ghost primitive
    temporary_primitive_.release();
  }
  
  void Painter::MouseMove(int x, int y) {
    // Set the correct ghost primitive if possible
    if (primitive_string_ == "Line" || primitive_string_ == "Box") {
      temporary_primitive_.reset(GetCurrentPrimitive(x, y));
    }
  }

  void Painter::KeyPressed(unsigned char ch, int x, int y) {
    switch(ch) {
      case 'p':
        primitive_string_ = "Point";
        break;
      case 'l':
        primitive_string_ = "Line";
        break;
      case 'b':
        primitive_string_ = "Box";
        break;
      case '1':
        color_string_ = "White";
        break;
      case '2':
        color_string_ = "Red";
        break;
      case '3':
        color_string_ = "Green";
        break;
      case '4':
        color_string_ = "Blue";
        break;
      case 'h':
        std::cout << "Help" << std::endl
                  << "Shapes: p Point, l Line, b Box" << std::endl
                  << "Colors: 1 Black, 2 Red, 3 Green, 4 Blue" << std::endl;
        break;
      default:
        break;
    }
  }
}
