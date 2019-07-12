#include <cstring>

#include <QtWidgets>

#include "GphotoCameraWidget.h"
#include "QGphotoWidget.h"

int cameraSettingUpdatedEvent;

class QGphotoWidgetWindow : public QGphotoWidget {
  private:
    QGphotoWidgetWindow(GphotoCameraWidget &cWidget) {
        tabWidget = new QTabWidget();
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                QWidget *child_widget = create_qgphoto_widget(child);
                if (child_widget != NULL)
                    tabWidget->addTab(child_widget, child.get_label());
            }
        }
        auto layout = new QStackedLayout(this);
        layout->addWidget(tabWidget);
    }
    void loadValue() {
	for (int i = 0; i < tabWidget->count(); i++) {
	    auto item = (QGphotoWidget*)tabWidget->widget(i);
	    item->loadValue();
	}
    }
    QTabWidget *tabWidget;
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
        auto widget = new QWidget();
        widget->setLayout(vbox);
        auto scrollArea = new QScrollArea();
        scrollArea->setWidget(widget);

        auto layout = new QStackedLayout(this);
        layout->addWidget(scrollArea);
    }
    void loadValue() {
	for (int i = 0; i < vbox->count(); i++) {
	    auto item = (QGphotoWidget*)(vbox->itemAt(i)->widget());
	    item->loadValue();
	}
    }
    QVBoxLayout *vbox;
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetRange : public QGphotoWidget {
  private:
    QGphotoWidgetRange(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        slider = new QSlider(Qt::Horizontal);
        auto layout = new QStackedLayout(this);
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
        auto layout = new QStackedLayout(this);
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

class QGphotoWidgetMenu : public QGphotoWidget {
  private:
    QGphotoWidgetMenu(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        comboBox = new QComboBox();
        auto layout = new QStackedLayout(this);
        layout->addWidget(comboBox);
        loadValue();
	QObject::connect(comboBox, QOverload<const QString &>::of(&QComboBox::activated), [=](const QString &text) {
	    this->cWidget.set_value<char *>(text.toUtf8().data());
            loadAllValues();
	});
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

class QGphotoWidgetButton : public QGphotoWidget {
  private:
    QGphotoWidgetButton(GphotoCameraWidget &cWidget) {
        auto layout = new QStackedLayout(this);
        layout->addWidget(new QPushButton(cWidget.get_label()));
    }
    void loadValue() {}
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget) {
    switch (cWidget.get_type()) {
    case GP_WIDGET_WINDOW:
        return new QGphotoWidgetWindow(cWidget);
    case GP_WIDGET_SECTION: {
        auto widget = new QGphotoWidgetSection(cWidget);
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
