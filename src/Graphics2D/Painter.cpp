#include <iostream>
#include <Graphics2D/Painter.hh>
#include <Graphics2D/PrimitivePolygon.hh>
#include <Graphics2D/PrimitivePoint.hh>
#include <Graphics2D/PrimitiveLine.hh>
#include <Graphics2D/PrimitiveBox.hh>
#include <Graphics2D/PrimitiveStar.hh>
#include <Graphics2D/Coordinate.hh>
#include <Graphics2D/Histogram.hh>
#include <Graphics2D/ColorConversion.hh>
#include <Graphics2D/BackgroundImage.hh>

namespace Graphics2D {

  Painter::Painter() {
    drawMode_ = line;
    togrey_ = false;
    enhance_ = false;
    drawHisto_ = false;
    color_ = Color::black();
    previewPrimitive_ = NULL;
  }

  Painter::~Painter() {
    for (unsigned int i=0;i<primitives_.size();i++) {
      delete primitives_[i];
      primitives_[i] = NULL;
    }
    if (previewPrimitive_ != NULL) {
      delete previewPrimitive_;
      previewPrimitive_ = NULL;
    }
  }
  
  void Painter::AddPrimitive(PrimitiveBase *primitive) {
    primitives_.push_back(primitive);
  }
  
  void Painter::Draw() {
    if (primitives_.size() > 0) {
      if (dynamic_cast<BackgroundImage*>(primitives_[0])) {
        primitives_[0]->Draw(image_);
      } else {
        std::cout << "first primitive must be background image" << std::endl;
        return;
      }
    }
    image_->SetColorModel(ImageBase::cm_RGB);
    Image *pImage = dynamic_cast<Image*>(image_);
    if (pImage == NULL) {
      std::cout << "can only convert Image Objects." << std::endl;
      return;
    }
    if (enhance_) {
      Histogram histo;
      histo.Autocontrast(*pImage, *pImage);
    } else if (togrey_) {
      ColorConversion::ToGrey(*pImage, *pImage);
    }
    
    Image white;
    white.Init(1024,768);
    memset(white.GetData(), 255, 1024*768*3);
    
    std::vector<Histogram> histogram(6);
    if (drawHisto_) {
      int w = image_->GetWidth() - 40;
      int h = (image_->GetHeight() - 120) / 7;
      w = 1024 - 40;
      h = (768 - 120) / 7;
      histogram[0].SetColor(Color::red());
      histogram[0].FromImage(*pImage, 0);
      
      histogram[1].SetColor(Color::green());
      histogram[1].FromImage(*pImage, 1);
      
      histogram[2].SetColor(Color::blue());
      histogram[2].FromImage(*pImage, 2);
      
      Image hsvImage;
      ColorConversion::ToHSV(*pImage, hsvImage);
      
      histogram[3].SetColor(Color::black());
      histogram[3].FromImage(hsvImage, 0);
      
      histogram[4].SetColor(Color::yellow());
      histogram[4].FromImage(hsvImage, 1);
      
      histogram[5].SetColor(Color::white());
      histogram[5].FromImage(hsvImage, 2);
      
      std::vector<Coordinate> coo(2);
      coo[1].Set( w, h );
      for (int i=0;i<6;i++) {
        coo[0].Set( 20, 20+i*(20+h) );
        histogram[i].SetCoordinates(coo);
      }
    }
    
    float rotAngle = float(0.25 * 2.0*M_PI / 25.0);
    for (unsigned int i=1;i<primitives_.size();i++) {
      primitives_[i]->Rotate(rotAngle);
      primitives_[i]->Draw(image_);
    }
    if (previewPrimitive_ != NULL) {
      previewPrimitive_->Draw(image_);
    }
    if (drawHisto_) {
      for (unsigned int i=0;i<histogram.size();i++) {
//        histogram[i].Draw(image_);
        histogram[i].Draw(&white);
      }
      white.SavePPM("histogramm6d.ppm");
    }
  }

  void Painter::MouseDown(int x, int y) {
    if (previewPrimitive_ != NULL) {
      coo_.push_back(Coordinate(x,y));
      previewPrimitive_->SetCoordinates(coo_);
      return;
    } else {
      coo_.clear();
    }
    switch (drawMode_) {
      case point: {
        PrimitivePoint *p = new PrimitivePoint;
        p->SetColor(color_);
        p->SetCoordinate( Coordinate(x,y) );
        primitives_.push_back(p);
        return;
      }
      case line:
        previewPrimitive_ = new PrimitiveLine;
        coo_.resize(2, Coordinate(x,y));
        break;
      case box:
        previewPrimitive_ = new PrimitiveBox;
        coo_.resize(2, Coordinate(x,y));
        break;
      case polygon:
        previewPrimitive_ = new PrimitivePolygon;
        coo_.resize(2, Coordinate(x,y));
        break;
      case star:
        previewPrimitive_ = new PrimitiveStar;
        coo_.resize(2, Coordinate(x,y));
        break;
      default:
        break;
    }
    previewPrimitive_->SetColor(color_);
    previewPrimitive_->SetCoordinates(coo_);
  }
  
  void Painter::MouseUp(int x, int y) {
    bool addPrimitive = false;
    switch (drawMode_) {
      case point:
        break;
      case line:
      case box:
      case star:
        coo_[ (int)coo_.size() -1 ] = Coordinate(x,y);
        addPrimitive = true;
        break;
      case polygon:
        coo_[ (int)coo_.size() -1 ] = Coordinate(x,y);
        previewPrimitive_->SetCoordinates(coo_);
        if (dynamic_cast<PrimitivePolygon*>(previewPrimitive_)->closed()) {
          addPrimitive = true;
        }
        break;
      default:
        break;
    }
    if (addPrimitive && previewPrimitive_ != NULL) {
      primitives_.push_back( previewPrimitive_ );
      previewPrimitive_ = NULL;
    }
  }
  
  void Painter::MouseMove(int x, int y) {
    if (previewPrimitive_ != NULL && coo_.size()>1) {
      coo_[ (int)coo_.size() -1 ] = Coordinate(x,y);
      previewPrimitive_->SetCoordinates(coo_);
    }
  }

  void Painter::KeyPressed(unsigned char ch, int x, int y) {
    switch (ch) {
      case '1':
    	color_ = Color::black();
        break;
      case '2':
        color_ = Color::white();
        break;
      case '3':
        color_ = Color::red();
        break;
      case '4':
        color_ = Color::green();
        break;
      case '5':
        color_ = Color::blue();
        break;
      case 'l':
        drawMode_ = line;
        break;
      case 'p':
        drawMode_ = point;
        break;
      case 'o':
        drawMode_ = polygon;
        break;
      case 'b':
        drawMode_ = box;
        break;
      case 's':
        drawMode_ = star;
        break;
      case 'c':
        togrey_ = !togrey_;
        if (togrey_)
          enhance_ = false;
        break;
      case 'k':
        enhance_ = !enhance_;
        if (enhance_)
          togrey_ = false;
        break;
      case 'j':
        drawHisto_ = !drawHisto_;
        if (drawHisto_)
          togrey_ = false;
        break;
      case ' ':
        {
          unsigned char *data = image_->GetData()+(image_->GetWidth()*y+x)*3;
          std::cout << (int)*(data) << " " << (int)*(data+1) << " " << (int)*(data+2) << std::endl;
        }
        break;
      case 'h':
      default:
        std::cout << " paint2d help: " << std::endl;
        std::cout << "\t[1-5]\tSelect colors" << std::endl;
        std::cout << "\tP\tDraw Points" << std::endl;
        std::cout << "\tL\tDraw Lines" << std::endl;
        std::cout << "\tB\tDraw Boxes" << std::endl;
        std::cout << "\tO\tDraw Polygons" << std::endl;
        std::cout << "\tS\tDraw Stars" << std::endl;
        std::cout << std::endl;
        std::cout << "\tC\tSwitch rgb, hsv, grey" << std::endl;
        std::cout << "\tK\tAuto Kontrast" << std::endl;
        std::cout << "\tJ\tDraw Histogram" << std::endl;
        std::cout << "\tspace\tShow current color value" << std::endl;
        break;
    }
  }
  
  std::string Painter::GetColorString() {
    return color_.name();
  }
  std::string Painter::GetString() {
    switch(drawMode_) {
      case line:
        return "Line";
      case point:
        return "Point";
      case polygon:
        return "Polygon";
      case box:
        return "Box";
      case star:
        return "Star";
      default:
        return "no primitive selected";
    }
  }

}
