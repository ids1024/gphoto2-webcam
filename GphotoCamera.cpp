#include "GphotoCamera.h"
#include "gpCall.h"

GphotoCamera::GphotoCamera(GphotoContext ctx) : ctx(ctx) {
    gpCall(gp_camera_new, &camera);
}

GphotoCamera::GphotoCamera(const GphotoCamera &rhs)
    : ctx(rhs.ctx), camera(rhs.camera) {
    gp_camera_ref(camera);
}

GphotoCamera::~GphotoCamera() {
    gp_camera_unref(camera);
}
GphotoCameraWidget GphotoCamera::get_config() {
    CameraWidget *window;
    gpCall(gp_camera_get_config, camera, &window, ctx.ctx);
    return GphotoCameraWidget(window);
}

void GphotoCamera::init() {
    gpCall(gp_camera_init, camera, ctx.ctx);
}

GphotoCameraFile GphotoCamera::capture_preview() {
    GphotoCameraFile file;
    gp_camera_capture_preview(camera, file.file, ctx.ctx);
    return file;
}
