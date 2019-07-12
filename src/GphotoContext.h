#ifndef GPHOTO_CONTEXT_H
#define GPHOTO_CONTEXT_H

#include <gphoto2/gphoto2.h>

class GphotoContext {
  public:
    GphotoContext();
    GphotoContext(const GphotoContext &rhs);
    ~GphotoContext();
    GphotoContext &operator=(const GphotoContext &) = delete;

  private:
    GPContext *ctx;
    friend class GphotoCamera;
    friend class GphotoCameraWidget;
};

#endif
