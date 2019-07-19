#include <cstring>

#include <QtWidgets>

#include "GphotoCameraWidget.h"
#include "QGphotoWidget.h"

using std::unique_ptr;
using std::make_unique;
using std::optional;

int cameraSettingUpdatedEvent;

class QGphotoWidgetWindow : public QGphotoWidget {
  private:
    QGphotoWidgetWindow(GphotoCameraWidget &cWidget) {
        tabWidget = new QTabWidget();
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                auto child_widget = create_qgphoto_widget(child);
                if (child_widget)
                    tabWidget->addTab(child_widget->release(), child.get_label());
            }
        }
        auto layout = new QStackedLayout(this);
        layout->addWidget(tabWidget);
    }
    void loadValue() {
	for (int i = 0; i < tabWidget->count(); i++) {
	    auto item = dynamic_cast<QGphotoWidget*>(tabWidget->widget(i));
	    item->loadValue();
	}
    }
    QTabWidget *tabWidget;
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetSection : public QGphotoWidget {
  private:
    QGphotoWidgetSection(GphotoCameraWidget &cWidget) {
        vbox = new QVBoxLayout;
        for (GphotoCameraWidget child : cWidget.get_children()) {
            if (!child.get_readonly()) {
                auto child_widget = create_qgphoto_widget(child);
                if (child_widget)
                    vbox->addWidget(child_widget->release());
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
	    auto item = dynamic_cast<QGphotoWidget*>(vbox->itemAt(i)->widget());
	    item->loadValue();
	}
    }
    QVBoxLayout *vbox;
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetRange : public QGphotoWidget {
  private:
    QGphotoWidgetRange(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        slider = new QSlider(Qt::Horizontal);
        auto layout = new QStackedLayout(this);
        layout->addWidget(slider);
        loadValue();
    }
    void loadValue() final {
        float min, max, increment;
        cWidget.get_range(&min, &max, &increment);
        slider->setRange(int(min / increment), int(max / increment));
        slider->setValue(int(cWidget.get_value<float>() / increment));
    }
    GphotoCameraWidget cWidget;
    QSlider *slider;
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetToggle : public QGphotoWidget {
  private:
    QGphotoWidgetToggle(GphotoCameraWidget &cWidget) : cWidget(cWidget) {
        checkBox = new QCheckBox(cWidget.get_label());
        auto layout = new QStackedLayout(this);
        layout->addWidget(checkBox);
        loadValue();
    }
    void loadValue() final {
        checkBox->setChecked(cWidget.get_value<bool>());
    }
    GphotoCameraWidget cWidget;
    QCheckBox *checkBox;
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
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
    void loadValue() final {
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
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

class QGphotoWidgetButton : public QGphotoWidget {
  private:
    QGphotoWidgetButton(GphotoCameraWidget &cWidget) {
        auto layout = new QStackedLayout(this);
        layout->addWidget(new QPushButton(cWidget.get_label()));
    }
    void loadValue() {}
    friend optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

optional<unique_ptr<QGphotoWidget>> create_qgphoto_widget(GphotoCameraWidget &cWidget) {
    switch (cWidget.get_type()) {
    case GP_WIDGET_WINDOW:
        return unique_ptr<QGphotoWidgetWindow>(new QGphotoWidgetWindow(cWidget));
    case GP_WIDGET_SECTION: {
        auto widget = unique_ptr<QGphotoWidgetSection>(new QGphotoWidgetSection(cWidget));
        if (widget->vbox->count() == 0) {
            return {};
        }
        return widget;
    }
    case GP_WIDGET_TEXT:
        return {};
    case GP_WIDGET_RANGE:
        return unique_ptr<QGphotoWidgetRange>(new QGphotoWidgetRange(cWidget));
    case GP_WIDGET_TOGGLE:
        return unique_ptr<QGphotoWidgetToggle>(new QGphotoWidgetToggle(cWidget));
    case GP_WIDGET_RADIO:
    case GP_WIDGET_MENU:
        return unique_ptr<QGphotoWidgetMenu>(new QGphotoWidgetMenu(cWidget));
    case GP_WIDGET_BUTTON:
        return unique_ptr<QGphotoWidgetButton>(new QGphotoWidgetButton(cWidget));
    case GP_WIDGET_DATE:
        return {};
    default:
        return {};
    }
}
