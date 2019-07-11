#include <exception>
#include <gphoto2/gphoto2.h>
#include <stdio.h>
#include <string>

#include "V4l2Loopback.h"

#define WIDTH 960
#define HEIGHT 640

class Gphoto2Error : public std::exception {
  private:
    std::string message;

  public:
    Gphoto2Error(std::string func, int err) {
        message = func + ": " + std::string(gp_result_as_string(err));
    }
    const char *what() const throw() {
        return message.c_str();
    }
};

template <typename F, typename... Args>
void _gpCall(const char *name, F f, Args... args) {
    int ret = f(args...);
    if (ret != GP_OK) {
        throw Gphoto2Error(name, ret);
    }
}
#define gpCall(f, ...) ((_gpCall(#f, ((f)), __VA_ARGS__)))

void capture_preview(Camera *camera, CameraFile **file, const char **data,
                     unsigned long int *size, GPContext *ctx) {
    gpCall(gp_file_new, file);
    gp_camera_capture_preview(camera, *file, ctx);
    gpCall(gp_file_get_data_and_size, *file, data, size);
}

int main() {
    GPContext *ctx = gp_context_new();

    Camera *camera;
    gpCall(gp_camera_new, &camera);

    gpCall(gp_camera_init, camera, ctx);

    CameraWidget *window;
    gpCall(gp_camera_get_config, camera, &window, ctx);
    for (int i = 0; i < gp_widget_count_children(window); i++) {
        CameraWidget *child;
        gpCall(gp_widget_get_child, window, i, &child);
        const char *name;
        gp_widget_get_name(child, &name);
        printf("%s\n", name);
    }

    V4l2Loopback loopback("/dev/video0");
    loopback.set_vidformat(HEIGHT, WIDTH);

    for (;;) {
        CameraFile *file;
        const char *data;
        unsigned long int size;
        capture_preview(camera, &file, &data, &size, ctx);
        loopback.write_frame(data, size);
        gp_file_unref(file);
    }
}
