
ADD_LIBRARY(Graphics2D 
    Image.cpp 
    Image.hh
    Color.hh
    Coordinate.cpp
    Coordinate.hh
    PrimitiveBase.cpp
    PrimitiveBase.hh
    PrimitivePoint.cpp
    PrimitivePoint.hh
    PrimitiveLine.cpp
    PrimitiveLine.hh
    PrimitivePolygon.cpp
    PrimitivePolygon.hh
    PrimitiveBox.cpp
    PrimitiveBox.hh
    PrimitiveStar.cpp
    PrimitiveStar.hh
    
    BackgroundImage.cpp
    BackgroundImage.hh
    Painter.cpp
    Painter.hh
    
    ColorConversion.cpp
    ColorConversion.hh
    Histogram.cpp
    Histogram.hh
    
    Filter.cpp
    Filter.hh

    Segmentation.cpp
    Segmentation.hh
    
    StructureTensor.cpp
    StructureTensor.hh
    
    HoughTransform.cpp
    HoughTransform.hh
    
    )

TARGET_LINK_LIBRARIES(Graphics2D Graphics2DBase)
