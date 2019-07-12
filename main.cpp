#include <exception>
#include <stdio.h>
#include <string>

#include <QtWidgets>

#include "V4l2Loopback.h"
#include "GphotoCamera.h"
#include "GphotoCameraFile.h"
#include "GphotoCameraWidget.h"

#define WIDTH 960
#define HEIGHT 640

int main() {
    GphotoContext ctx;

    GphotoCamera camera(ctx);
    camera.init();

    GphotoCameraWidget window = camera.get_config();
    for (GphotoCameraWidget child : window.children()) {
        printf("%s\n", child.get_name());
    }

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
