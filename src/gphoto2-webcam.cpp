#include <QtWidgets>

#include "GphotoCamera.h"
#include "GphotoCameraFile.h"
#include "GphotoCameraWidget.h"
#include "V4l2Loopback.h"
#include "QGphotoWidget.h"

const unsigned WIDTH = 960;
const unsigned HEIGHT = 640;

void setup_v4l(QApplication& app, const char *path, GphotoCamera *camera) {
    // XXX should be able to pass GphotoCamera as value; results in double free
    V4l2Loopback loopback(path);
    loopback.set_vidformat(HEIGHT, WIDTH);
    auto timer = new QTimer();
    timer->setInterval(30);
    app.connect(timer, &QTimer::timeout, &app, [=]() mutable {
        const char *data;
        unsigned long int size;
        GphotoCameraFile file = camera->capture_preview();
        file.get_data_and_size(&data, &size);
        loopback.write_frame(data, size);
    });
    timer->start();
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    cameraSettingUpdatedEvent = QEvent::registerEventType();

    GphotoContext ctx;
    GphotoCamera camera(ctx);
    camera.init();

    // XXX path
    setup_v4l(app, "/dev/video0", &camera);

    GphotoCameraWidget config = camera.get_config();
    auto window = *create_qgphoto_widget(config);

    window->show();
    window->setWindowTitle("Gphoto2-Webcam");
    return app.exec();
}
