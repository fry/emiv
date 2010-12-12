#include <Graphics2DBase/Color.hh>

namespace Graphics2D {
  Color::Color(const std::string& name, unsigned char r, unsigned char g, unsigned char b):
    name_(name), r_(r), g_(g), b_(b) {
  }
}
