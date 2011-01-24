/*
 * Image Class implementing ImageBase
 * @author fkellner, 10/10
 */

#include <cstdio>
#include <string.h>
#include <fstream>
#include <iostream>
#include <Graphics2D/Image.hh>

using namespace std;

namespace Graphics2D {

  Image::Image() {
  }
  
  Image::~Image() {
  }
  
  void Image::FillZero() {
    if (Valid()) {
      // could also be done in a for loop
      memset(data_, 0, width_*height_*3);
    }
  }
  
  int Image::LoadPPM(const string &filename) {
    // open file
    ifstream file;
    file.open(filename.c_str(),ios::in | ios::binary);
    // check if file is opened
    if (!file.good()) {
      cout << "file not found" << endl;
      return -1;
    }
    // we need some buffer for the comment and header
    string buffer;
    // get header
    getline(file, buffer);
    if (!(buffer[0] == 'P' && (buffer[1] == '3' || buffer[1] == '6'))) {
      cout << "Unrecognized ppm format." << endl;
      return -1;
    }
    // check if P3 or P6 image
    bool plain = true;
    if (buffer[0] == 'P' && buffer[1] == '6') {
      plain = false;
    }
    // check if for a line which is not a comment (starting with #)
    char testcomment = file.peek();
    while (testcomment == '#') {
      getline(file, buffer);
      if (buffer.compare("# cm_RGB")==0) {
        colormodel_ = cm_RGB;
      } else if (buffer.compare("# cm_Grey")==0) {
        colormodel_ = cm_Grey;
      } else if (buffer.compare("# cm_HSV")==0) {
        colormodel_ = cm_HSV;
      }
      testcomment = file.peek();
    }
    // dummy is used because we only use uchar images (max val 255)
    unsigned int w,h,dummy;
    file >> w >> h >> dummy;
    // create memory for w*h image
    Init(w,h);
    
    if (plain) {
      // image is in ascii format
      int tmp;
      for (unsigned int x=0;x<width_*height_*3;x++) {
        // read int value of current word
        file >> tmp;
        // store as unsigned char
        data_[x] = (unsigned char)tmp;
      }
    } else {
      // jump to next line, which has our byte data
      getline(file, buffer);
      for (unsigned int x=0;x<width_*height_*3;x++) {
        // store next byte
        file.get((char&)data_[x]);
      }
    }
    file.close();
    return 0;
  }
  
  int Image::SavePPM(const string &filename) {
    if (!Valid()) {
      cout << "image is empty in save image" << endl;
      return -1;
    }
    // open file
    ofstream file;
    file.open(filename.c_str(),ios::out | ios::binary);
    // check if file is ready for writing
    if (!file.good()) {
      cout << "file not writable" << endl;
      return -1;
    }
    // write header (with comment)
    file << "P6" << endl;
    switch (colormodel_) {
      case cm_Grey:
        file << "# cm_Grey" << endl;
        break;
      case cm_RGB:
        file << "# cm_RGB" << endl;
        break;
      case cm_HSV:
        file << "# cm_HSV" << endl;
        break;
      default:
        file << "# mip" << endl;
        break;
    }
    file << width_ << " " << height_ << endl;
    file << "255" << endl;
    // write bytewise into the file
    for (unsigned int y=0;y<height_;y++) {
      for (unsigned int x=0;x<width_*3;x++) {
        file << data_[(y*width_*3)+x];
      }
    }
    file.close();
    return 0;
  }
  
}
