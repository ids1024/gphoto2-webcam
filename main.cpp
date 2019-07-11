#include <exception>
#include <stdio.h>
#include <string>

#include "gphoto2cpp.h"
#include "V4l2Loopback.h"

#define WIDTH 960
#define HEIGHT 640

int main() {
    GphotoContext ctx;

    GphotoCamera camera(ctx);
    camera.init();

    /*
    CameraWidget *window;
    gpCall(gp_camera_get_config, camera, &window, ctx);
    for (int i = 0; i < gp_widget_count_children(window); i++) {
        CameraWidget *child;
        gpCall(gp_widget_get_child, window, i, &child);
        const char *name;
        gp_widget_get_name(child, &name);
        printf("%s\n", name);
    }
    */

    V4l2Loopback loopback("/dev/video0");
    loopback.set_vidformat(HEIGHT, WIDTH);

    for (;;) {
        const char *data;
        unsigned long int size;
        GphotoCameraFile file = camera.capture_preview();
        file.get_data_and_size(&data, &size);
        loopback.write_frame(data, size);
    }
}
