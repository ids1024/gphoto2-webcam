#ifndef QGPHOTO_WIDGET_H
#define QGPHOTO_WIDGET_H

extern int cameraSettingUpdatedEvent;

class QGphotoWidget : public QWidget {
  public:
    virtual void loadValue() = 0;
  protected:
    QGphotoWidget(){};
    bool event(QEvent *event) {
        if (event->type() == cameraSettingUpdatedEvent) {
            loadValue();
            return true;
        }
        return QWidget::event(event);
    }
    void loadAllValues() {
        QEvent event((QEvent::Type)cameraSettingUpdatedEvent);
        QApplication::sendEvent(window(), &event);
    }
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);

#endif
