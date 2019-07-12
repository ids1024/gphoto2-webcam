#ifndef GPHOTO_CAMERA_WIDGET_H
#define GPHOTO_CAMERA_WIDGET_H

#include <gphoto2/gphoto2.h>
#include <iterator>

class GphotoCameraWidget {
  public:
    GphotoCameraWidget(const GphotoCameraWidget &rhs);
    ~GphotoCameraWidget();
    GphotoCameraWidget &operator=(const GphotoCameraWidget &) = delete;
    const char *get_name();
    bool get_readonly();

    class Children;
    Children children();

    class child_iterator
        : public std::iterator<std::input_iterator_tag, GphotoCameraWidget> {
      public:
        child_iterator &operator++();
        bool operator==(const child_iterator &rhs) const;
        bool operator!=(const child_iterator &rhs) const;
        GphotoCameraWidget operator*();

      private:
        child_iterator(CameraWidget *widget, int n);
        CameraWidget *widget;
        int n;
        friend class Children;
    };

    class Children {
      public:
        child_iterator begin();
        child_iterator end();

      private:
        Children(CameraWidget *widget);
        CameraWidget *widget;
        friend class GphotoCameraWidget;
    };

  private:
    GphotoCameraWidget(CameraWidget *widget);
    CameraWidget *widget;
    friend class GphotoCamera;
};

#endif
