# Gruppe 8
# Serie 2
# Alexander Koch, 773036
# Eike Siewertsen, 847522

add_library(Graphics2D
  Image.cpp
  Image.hh
  PrimitivePoint.hh
  PrimitivePoint.cpp
  PrimitiveLine.hh
  PrimitiveLine.cpp
  PrimitivePolygon.hh
  PrimitivePolygon.cpp
  PrimitiveBox.hh
  PrimitiveBox.cpp
  PrimitiveStar.cpp
  Painter.hh
  Painter.cpp
  ColorConversion.cpp
  Histogram.cpp)
target_link_libraries(Graphics2D Graphics2DBase)
