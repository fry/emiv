#include <Graphics2D/Histogram.hh>
#include <Graphics2D/ColorConversion.hh>
#include <cmath>
#include <memory.h>

using namespace Graphics2D;

Histogram::Histogram(const Color& color, const Coordinate& pos, const Coordinate& size): PrimitiveBase(color) {
  std::vector<Coordinate> coords;
  coords.reserve(2);
  coords.push_back(pos);
  coords.push_back(size);
  
  coordinates_ = coords;
}

void Histogram::FromImage(const Image& image, const int& channel) {
  memset(values, 0, 256 * 4);
  for (int x = 0; x < image.GetWidth(); x++) {
    for (int y = 0; y < image.GetHeight(); y++) {
      unsigned char value = image.GetPixel(x, y, channel);
      values[value]++;
    }
  }
  
  UpdateBoxes();
}

int Histogram::GetMin() {
  for (int i = 0; i < 256; i++) {
    if (values[i] != 0)
      return i;
  }
  
  assert(false);
}

int Histogram::GetMax() {
  for (int i = 256 - 1; i >= 0; i--) {
    if (values[i] != 0)
      return i;
  }
  
  assert(false);
}

void Histogram::Autocontrast(const Image& src, Image& dst) {
  const int min = GetMin();
  const int max = GetMax();
  const int diff = max - min;
  const float fac = 255.0 / diff;
  
  if (diff == 0)
    return;

  for (int x = 0; x < src.GetWidth(); x++) {
    for (int y = 0; y < src.GetHeight(); y++) {
      unsigned char new_val = static_cast<unsigned char>(fac * (src.GetPixel(x, y, 2) - min));
      dst.SetPixel(x, y, 2, new_val);
    }
  }
}

void Histogram::SetCoordinates(const std::vector<Coordinate>& coordinates) {
  if (coordinates.size() != 2)
    throw std::runtime_error("Histogram requires two coordinates");
  
  PrimitiveBase::SetCoordinates(coordinates);
}

void Histogram::Draw(ImageBase* im) {
  std::vector<PrimitiveBox>::iterator iter, end;
  end = boxes.end();
  
  for (iter = boxes.begin(); iter != end; ++iter) {
    iter->Draw(im);
  }
}

void Histogram::UpdateBoxes() {
  boxes.clear();

  // position is x/y
  const Coordinate& position = coordinates_[0];
  // size is width/height
  const Coordinate& size = coordinates_[1];
  
  // Calculate maximum value
  int max = 0;
  for (int i = 0; i < 256; i++) {
    max = std::max(values[i], max);
  }
  
  // calculate max box size and width
  const float s = size.GetY() / log10(max + 1);
  const float box_width = size.GetX() / 256.0;
  
  // specify bottom y of the boxes
  const float y = position.GetY() + size.GetY();
  boxes.reserve(256);
  
  float current_x = position.GetX();
  // generate primitive boxes
  for (int i = 0; i < 256; i++) {
    const float height = s * log10(values[i] + 1);
    // Create the box with the precondition that the coordinate system origin is at the top left
    PrimitiveBox box(GetColor(), Coordinate(current_x, y), Coordinate(current_x + box_width, y - height));
    boxes.push_back(box);
    
    current_x += box_width;
  }
}
