/* 
 This file is part of the BIAS library (Basic ImageAlgorithmS).

 Copyright (C) 2003, 2004    (see file CONTACTS for details)
 Multimediale Systeme der Informationsverarbeitung
 Institut fuer Informatik
 Christian-Albrechts-Universitaet Kiel


 BIAS is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 BIAS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with BIAS; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <Graphics2D/BackgroundImage.hh>
#include <vector>

using std::min;

namespace Graphics2D {

  BackgroundImage::BackgroundImage(Image &im) {
    source_ = im;
    name_ = "BackgroundImage";
    coords_.clear();
  }
  
  BackgroundImage::BackgroundImage(std::string filename) {
    source_.LoadPPM(filename);
    name_ = "BackgroundImage";
    coords_.clear();
  }

  BackgroundImage::~BackgroundImage() {
    // TODO Auto-generated destructor stub
  }
  
  void BackgroundImage::Draw(ImageBase *im) {
    unsigned int w=min(source_.GetWidth(), im->GetWidth());
    unsigned int h=min(source_.GetHeight(), im->GetHeight());
    unsigned char *src = source_.GetData();
    unsigned char *dest = im->GetData();
    for (unsigned int x=0;x<w*h*3;x++) {
      dest[x] = src[x];
    }
  }

}
