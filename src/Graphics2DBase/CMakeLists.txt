SET(GRAPHICS2DBASE_SRC
    ImageBase.cpp 
    ImageBase.hh
    PainterBase.hh
    Canvas2D.hh
    FloatImage.cpp
    FloatImage.hh
)


ADD_LIBRARY(Graphics2DBase 
    ${GRAPHICS2DBASE_SRC}
)

TARGET_LINK_LIBRARIES(Graphics2DBase ${EXT_LIBRARIES})