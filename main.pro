SOURCES += main.cpp V4l2Loopback.cpp
SOURCES += GphotoCamera.cpp GphotoCameraFile.cpp GphotoCameraWidget.cpp GphotoContext.cpp
LIBS += -lgphoto2
QT += widgets
QMAKE_CXXFLAGS += --std=c++17
