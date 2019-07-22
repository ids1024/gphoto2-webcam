#ifndef GPHOTO_CAMERA_H
#define GPHOTO_CAMERA_H

#include "GphotoCameraFile.h"
#include "GphotoCameraWidget.h"
#include "GphotoContext.h"
#include "GphotoRefCount.h"

class GphotoCamera {
  public:
    GphotoCamera(GphotoContext ctx);
    GphotoCameraWidget get_config();
    void init();
    GphotoCameraFile capture_preview();
    void set_single_config(const char *name, GphotoCameraWidget &cWidget);
  private:
    GphotoContext ctx;
    GphotoRefCount<Camera, gp_camera_ref, gp_camera_unref> camera;
    friend class GphotoCameraWidget;
};

#endif
