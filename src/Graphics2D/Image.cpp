// Gruppe 8
// Serie 2
// Alexander Koch, 773036
// Eike Siewertsen, 847522

#include <Graphics2D/Image.hh>
#include <fstream>
#include <cstring>

using namespace Graphics2D;

namespace {
  template<class T> bool read(std::ifstream& file, T& out) {
    if (!file.good()) { //check if the file is accessible
      return false;
    }
    char peek = file.peek();
    if (peek == '#' || peek == '\n') { //skip comments and newlines
      std::string line;
      std::getline(file, line);
      return read(file, out);
    }
    
    file >> out;
    return true;
  }
}

void Image::FillZero() {
  memset(data_, 0, width_*height_*3);
}

int Image::LoadPPM(const std::string &filename) {
  std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
  if (!file.good())
    return -2;

  //read header
  std::string format;
  read(file, format);
  int width, height, maxValue;
  read(file, width);
  read(file, height);
  read(file, maxValue);
  
  Init(width, height);
  if (format == "P3") {
    // read rows of pixel data
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        for (int ch = 0; ch < 3; ch++) {
          unsigned int val;
          read(file, val);
          SetPixel(x, y, ch, val);
        }
      }
    }
  } else if (format == "P6") {
    // skip any remaining newlines and comments
    while (file.peek() == '#' || file.peek() == '\n') {
      std::string line;
      std::getline(file, line);
    }
    // read pixel data blob
    file.read(reinterpret_cast<char*>(data_), width*height*3);
  } else {
    return -1;
  }
  
  file.close();
  
  return 0;
}

int Image::SavePPM(const std::string &filename) {
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
  //file accessible?
  if (!file.good())
    return -2;
   
  //write header
  file << "P6" << std::endl;
  file << width_ << " " << height_ << std::endl;
  file << 255 << std::endl;
  //write pixeldata
  file.write(reinterpret_cast<const char*>(data_), width_*height_*3);
  
  file << std::endl;
  
  file.close();
  return 0;
}
