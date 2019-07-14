#ifndef GPHOTO_CONTEXT_H
#define GPHOTO_CONTEXT_H

#include <gphoto2/gphoto2.h>

#include "GphotoRefCount.h"

class GphotoContext {
  public:
    GphotoContext();
  private:
    GphotoRefCount<GPContext, gp_context_ref, gp_context_unref> ctx;
    friend class GphotoCamera;
    friend class GphotoCameraWidget;
};

#endif
