#include <Graphics2D/Painter.hh>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitiveLine.hh>
#include <Graphics2D/PrimitiveBox.hh>
#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/PrimitiveStar.hh>
#include <Graphics2D/ColorConversion.hh>

#include <cmath>

namespace Graphics2D {
  Painter::Painter(ImageBase* background): specified_background_(background),
    hist_red_(Color::red(), Coordinate(10,10), Coordinate(800,100)),
    hist_blue_(Color::blue(), Coordinate(10,100), Coordinate(800,100)),
    hist_green_(Color::green(), Coordinate(10,190), Coordinate(800,100)),
    
    hist_hue_(Color::white(), Coordinate(10,300), Coordinate(800,100)),
    hist_saturation_(Color::black(), Coordinate(10,390), Coordinate(800,100)),
    hist_value_(Color::yellow(), Coordinate(10,480), Coordinate(800,100)),
    
    display_histogram_(false)
  {    
    // Copy background
    ImageBase* my_bg = &background_;
    *my_bg = *background;
    
    UpdateHistogram();
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
      // Draw a box with its two points
      std::vector<Coordinate> boxCoords;
      boxCoords.push_back(Coordinate(draw_start_x, draw_start_y));
      boxCoords.push_back(Coordinate(x, y));
      return new PrimitiveBox(GetColor(), boxCoords);
    } else if (primitive_string_ == "Polygon") {
      // Only return a primitive for polygons if there's going to be
      // at least two points in it
      if (!polygon_points_.empty()) {
        std::vector<Coordinate> new_points = polygon_points_;
        new_points.push_back(Coordinate(x, y));
        return new PrimitivePolygon(GetColor(), new_points);
      }
    } else if (primitive_string_ == "Star") {
      const Coordinate vec(draw_start_x - x, draw_start_y - y);
      
      const float distance = std::sqrt(
        std::pow(vec.GetX(), 2.0) +
        std::pow(vec.GetY(), 2.0));
      
      const float angle = std::acos((float)vec.GetY() / distance);
      
      PrimitiveStar* star = new PrimitiveStar(GetColor(), Coordinate(draw_start_x, draw_start_y), distance);
      star->Rotate(angle);
      return star;
    }
    
    return NULL;
  }
  
  void Painter::Draw() {
    // Clear the image
    *image_ = background_;
    
    // Iterate through contained primitives and draw them
    std::vector<PrimitiveBase*>::iterator iter, end;
    end = primitives_.end();
    
    // Rotate and draw all primitives at fixed speed
    for (iter = primitives_.begin(); iter != end; ++iter) {
      (*iter)->Rotate(2*M_PI * (1.0/4.0 * 1.0/25.0));
      (*iter)->Draw(image_);
    }
    
    // If a "ghost primitive" is set, draw it
    if (temporary_primitive_.get() != 0)
      temporary_primitive_->Draw(image_);
      
    // Draw histograms
    if (display_histogram_) {
      hist_red_.Draw(image_);
      hist_green_.Draw(image_);
      hist_blue_.Draw(image_);
    
      hist_hue_.Draw(image_);
      hist_saturation_.Draw(image_);
      hist_value_.Draw(image_);
    }
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
    // Special handling for polygons
    if (primitive_string_ == "Polygon") {
      bool stop_plotting = false;
      
      // Determine whether we clicked near the start point of our polygon
      // we also can't finish a polygon if it only has one point
      if (polygon_points_.size() > 1) {
        const int dx = std::abs(x - polygon_points_[0].GetX());
        const int dy = std::abs(y - polygon_points_[0].GetY());
        
        stop_plotting = dx < 10 && dy < 10;
      }
      
      if (stop_plotting) {
        // We're supposed to stop plotting, add the finished polygon
        AddPrimitive(new PrimitivePolygon(GetColor(), polygon_points_));

        // Clear polygon point cache
        polygon_points_.clear();
      } else {
        polygon_points_.push_back(Coordinate(x, y));
      
        // MouseMove only gets called when the mouse is moved while a key is pressed,
        // so invoke it once here to update the ghost primitive
        MouseMove(x, y);
        return;
      }
    } else {
      // Add the finished primitive
      PrimitiveBase* prim = GetCurrentPrimitive(x, y);
      if (prim != NULL)
        AddPrimitive(prim);
    }
    
    // And get rid of any ghost primitive
    temporary_primitive_.release();
  }
  
  void Painter::MouseMove(int x, int y) {
    // Set the correct ghost primitive if possible
    if (primitive_string_ != "Point") {
      temporary_primitive_.reset(GetCurrentPrimitive(x, y));
    }
  }

  void Painter::KeyPressed(unsigned char ch, int x, int y) {
    Image* bg_img = dynamic_cast<Image*>(specified_background_);
    if (bg_img == 0)
      return;

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
      case 'o':
        primitive_string_ = "Polygon";
        break;
      case 's':
        primitive_string_ = "Star";
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
      case 'c':
        // convert between greyscale <-> rgb
        if (background_.GetColorModel() == ImageBase::cm_RGB) {
          ColorConversion::ToGrey(*bg_img, background_);
          UpdateHistogram();
        } else if (background_.GetColorModel() == ImageBase::cm_Grey) {
          background_ = *bg_img;
          background_.SetColorModel(ImageBase::cm_RGB);
          UpdateHistogram();
        }
        break;
      case 'x':
        // test convert to hsv and back to rgb
        ColorConversion::ToHSV(*bg_img, background_);
        //ColorConversion::ToRGB(background_, background_);
        
        UpdateHistogram();
        break;
      case 'k':
        // Convert the image to HSV, autocontrast the value of HSV and convert it back
        ColorConversion::ToHSV(background_, background_);
        hist_value_.Autocontrast(background_, background_);
        ColorConversion::ToRGB(background_, background_);
        
        UpdateHistogram();
        break;
      case ' ':
        std::cout << static_cast<int>(image_->GetPixel(x, y, 0)) << ", "
                  << static_cast<int>(image_->GetPixel(x, y, 1)) << ", "
                  << static_cast<int>(image_->GetPixel(x, y, 2)) << std::endl;
        break;
      case 'j':
        display_histogram_ = !display_histogram_;
        break;
      case 'h':
        std::cout << "Help" << std::endl
                  << "Shapes: p Point, l Line, b Box, o Polygon, s Star" << std::endl
                  << "Colors: 1 White, 2 Red, 3 Green, 4 Blue" << std::endl
                  << "Conversions: c Grey, x to HSV and back" << std::endl
                  << "k Autocontrast" << std::endl
                  << "j Show/Hide Histograms" << std::endl;
        break;
      default:
        break;
    }
  }
  
  void Painter::UpdateHistogram() {
    hist_red_.FromImage(background_, 0);
    hist_blue_.FromImage(background_, 1);
    hist_green_.FromImage(background_, 2);
   
    // Generate HSV image to extract histogram of
    Image background_hsv;
    background_hsv.Init(background_.GetWidth(), background_.GetHeight());
    ColorConversion::ToHSV(background_, background_hsv);
    
    hist_hue_.FromImage(background_hsv, 0);
    hist_saturation_.FromImage(background_hsv, 1);
    hist_value_.FromImage(background_hsv, 2);
  }
}
