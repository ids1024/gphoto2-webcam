#ifndef GPHOTO_CAMERA_H
#define GPHOTO_CAMERA_H

#include "GphotoContext.h"
#include "GphotoCameraWidget.h"
#include "GphotoCameraFile.h"

class GphotoCamera {
  public:
    GphotoCamera(GphotoContext ctx);
    GphotoCamera(const GphotoCamera &rhs);
    ~GphotoCamera();
    GphotoCamera &operator=(const GphotoCamera &) = delete;
    GphotoCameraWidget get_config();
    void init();
    GphotoCameraFile capture_preview();

  private:
    GphotoContext ctx;
    Camera *camera;
};

#endif
