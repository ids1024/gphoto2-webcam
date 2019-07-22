#include "GphotoCamera.h"
#include "gpCall.h"

inline Camera *camera_new() {
    Camera *camera;
    gpCall(gp_camera_new, &camera);
    return camera;
}

GphotoCamera::GphotoCamera(GphotoContext ctx) : ctx(ctx), camera(camera_new()) {}

GphotoCameraWidget GphotoCamera::get_config() {
    CameraWidget *window;
    gpCall(gp_camera_get_config, camera.get(), &window, ctx.ctx.get());
    return GphotoCameraWidget(window);
}

void GphotoCamera::init() {
    gpCall(gp_camera_init, camera.get(), ctx.ctx.get());
}

GphotoCameraFile GphotoCamera::capture_preview() {
    GphotoCameraFile file;
    gpCall(gp_camera_capture_preview, camera.get(), file.file.get(), ctx.ctx.get());
    return file;
}

void GphotoCamera::set_single_config(const char *name, GphotoCameraWidget &cWidget) {
    gpCall(gp_camera_set_single_config, camera.get(), name, cWidget.widget.get(), ctx.ctx.get());
}
