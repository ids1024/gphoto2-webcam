#ifndef QGPHOTO_WIDGET_H
#define QGPHOTO_WIDGET_H

class QGphotoWidget : public QWidget {
  protected:
    QGphotoWidget(){};
    friend QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);
};

QGphotoWidget *create_qgphoto_widget(GphotoCameraWidget &cWidget);

#endif
