#include <QtWidgets>

#include "GphotoCamera.h"
#include "GphotoCameraFile.h"
#include "GphotoCameraWidget.h"
#include "V4l2Loopback.h"
#include "QGphotoWidget.h"

#define WIDTH 960
#define HEIGHT 640

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    cameraSettingUpdatedEvent = QEvent::registerEventType();

    GphotoContext ctx;
    GphotoCamera camera(ctx);
    camera.init();

    GphotoCameraWidget config = camera.get_config();
    QWidget *window = create_qgphoto_widget(config);

    window->show();
    window->setWindowTitle("Gphoto2-Webcam");
    return app.exec();

    /*
    V4l2Loopback loopback("/dev/video0");
    loopback.set_vidformat(HEIGHT, WIDTH);

    for (;;) {
        const char *data;
        unsigned long int size;
        GphotoCameraFile file = camera.capture_preview();
        file.get_data_and_size(&data, &size);
        loopback.write_frame(data, size);
    }
    */
}
