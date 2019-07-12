SOURCES += src/gphoto2-webcam.cpp src/V4l2Loopback.cpp src/QGphotoWidget.cpp
SOURCES += src/GphotoCamera.cpp src/GphotoCameraFile.cpp src/GphotoCameraWidget.cpp src/GphotoContext.cpp
LIBS += -lgphoto2
QT += widgets
QMAKE_CXXFLAGS += --std=c++17
