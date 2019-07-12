#include <exception>
#include <stdio.h>
#include <string>

#include <QtWidgets>

#include "GphotoCamera.h"
#include "GphotoCameraFile.h"
#include "GphotoCameraWidget.h"
#include "V4l2Loopback.h"

#define WIDTH 960
#define HEIGHT 640

QWidget *create_config_widget(GphotoCameraWidget &cWidget) {
    switch (cWidget.get_type()) {
    case GP_WIDGET_WINDOW: {
        QTabWidget *widget = new QTabWidget();
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                QWidget *child_widget = create_config_widget(child);
                if (child_widget != NULL)
                    widget->addTab(child_widget, child.get_label());
            }
        }
        return widget;
    }
    case GP_WIDGET_SECTION: {
        QVBoxLayout *vbox = new QVBoxLayout;
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                QWidget *child_widget = create_config_widget(child);
                if (child_widget != NULL)
                    vbox->addWidget(create_config_widget(child));
            }
        }
        if (vbox->count() == 0) {
            delete vbox;
            return NULL;
        }
        QWidget *widget = new QWidget();
        widget->setLayout(vbox);
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidget(widget);
        return scrollArea;
    }
    case GP_WIDGET_TEXT:
        return NULL;
    case GP_WIDGET_RANGE: {
        float min, max, increment;
        cWidget.get_range(&min, &max, &increment);
        QSlider *widget = new QSlider(Qt::Horizontal);
        widget->setRange(int(min / increment), int(max / increment));
        widget->setValue(int(cWidget.get_value<float>() / increment));
        return widget;
    }
    case GP_WIDGET_TOGGLE: {
        QCheckBox *widget = new QCheckBox(cWidget.get_label());
        widget->setChecked(cWidget.get_value<bool>());
        return widget;
    }
    case GP_WIDGET_RADIO:
    case GP_WIDGET_MENU: {
        QComboBox *widget = new QComboBox();
        const char *selected = cWidget.get_value<const char *>();
        for (const char *choice : cWidget.get_choices()) {
            widget->addItem(choice);
            if (strcmp(choice, selected) == 0) {
                widget->setCurrentIndex(widget->count() - 1);
            }
        }
        return widget;
    }
    case GP_WIDGET_BUTTON:
        return new QPushButton(cWidget.get_label());
    case GP_WIDGET_DATE:
        return NULL;
    }

    return NULL;
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    GphotoContext ctx;

    GphotoCamera camera(ctx);
    camera.init();

    GphotoCameraWidget config = camera.get_config();
    QWidget *window = create_config_widget(config);

    window->show();
    window->setWindowTitle("Gphoto2-V4L2");
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
