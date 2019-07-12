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

class QGphotoWidget : public QWidget {
  protected:
    QGphotoWidget(){ };
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);

class QGphotoWidgetWindow : public QGphotoWidget {
  private:
    QGphotoWidgetWindow(GphotoCameraWidget &cWidget) {
        QTabWidget *widget = new QTabWidget();
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                QWidget *child_widget = create_qgphoto_widget(child);
                if (child_widget != NULL)
                    widget->addTab(child_widget, child.get_label());
            }
        }
        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(widget);
    }
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetSection : public QGphotoWidget {
  private:
    QGphotoWidgetSection(GphotoCameraWidget &cWidget) {
        vbox = new QVBoxLayout;
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                QWidget *child_widget = create_qgphoto_widget(child);
                if (child_widget != NULL)
                    vbox->addWidget(create_qgphoto_widget(child));
            }
        }
        QWidget *widget = new QWidget();
        widget->setLayout(vbox);
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidget(widget);

        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(scrollArea);
    }
    QVBoxLayout *vbox;
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetRange : public QGphotoWidget {
  private:
    QGphotoWidgetRange(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        slider = new QSlider(Qt::Horizontal);
        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(slider);
        loadValue();
    }
    void loadValue() {
        float min, max, increment;
        cWidget.get_range(&min, &max, &increment);
        slider->setRange(int(min / increment), int(max / increment));
        slider->setValue(int(cWidget.get_value<float>() / increment));
    }
    GphotoCameraWidget cWidget;
    QSlider *slider;
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetToggle : public QGphotoWidget {
  private:
    QGphotoWidgetToggle(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        checkBox = new QCheckBox(cWidget.get_label());
        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(checkBox);
        loadValue();
    }
    void loadValue() {
        checkBox->setChecked(cWidget.get_value<bool>());
    }
    GphotoCameraWidget cWidget;
    QCheckBox *checkBox;
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetMenu: public QGphotoWidget {
  private:
    QGphotoWidgetMenu(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        comboBox = new QComboBox();
        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(comboBox);
        loadValue();
    }
    void loadValue() {
        const char *selected = cWidget.get_value<const char *>();
        comboBox->clear();
        for (const char *choice : cWidget.get_choices()) {
            comboBox->addItem(choice);
            if (strcmp(choice, selected) == 0) {
                comboBox->setCurrentIndex(comboBox->count() - 1);
            }
        }
    }
    GphotoCameraWidget cWidget;
    QComboBox *comboBox;
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetButton: public QGphotoWidget {
  private:
    QGphotoWidgetButton(GphotoCameraWidget &cWidget) {
        QStackedLayout *layout = new QStackedLayout(this);
        layout->addWidget(new QPushButton(cWidget.get_label()));
    }
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget) {
    switch (cWidget.get_type()) {
    case GP_WIDGET_WINDOW:
        return new QGphotoWidgetWindow(cWidget);
    case GP_WIDGET_SECTION: {
        QGphotoWidgetSection *widget = new QGphotoWidgetSection(cWidget);
        if (widget->vbox->count() == 0) {
            delete widget;
            return NULL;
        }
        return widget;
    }
    case GP_WIDGET_TEXT:
        return NULL;
    case GP_WIDGET_RANGE:
        return new QGphotoWidgetRange(cWidget);
    case GP_WIDGET_TOGGLE:
        return new QGphotoWidgetToggle(cWidget);
    case GP_WIDGET_RADIO:
    case GP_WIDGET_MENU:
        return new QGphotoWidgetMenu(cWidget);
    case GP_WIDGET_BUTTON:
        return new QGphotoWidgetButton(cWidget);
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
    QWidget *window = create_qgphoto_widget(config);

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
