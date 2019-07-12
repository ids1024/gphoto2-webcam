#ifndef GPHOTO_CAMERA_WIDGET_H
#define GPHOTO_CAMERA_WIDGET_H

#include <gphoto2/gphoto2.h>
#include <iterator>

#include "GphotoError.h"

class GphotoCamera;

class GphotoCameraWidget {
  public:
    GphotoCameraWidget(const GphotoCameraWidget &rhs);
    ~GphotoCameraWidget();
    GphotoCameraWidget &operator=(const GphotoCameraWidget &) = delete;
    const char *get_name();
    const char *get_label();
    bool get_readonly();
    CameraWidgetType get_type();
    template <class T> T get_value() {
        T value;
        int ret = gp_widget_get_value(widget, &value);
        if (ret != GP_OK) {
            throw GphotoError("gp_widget_get_value", ret);
        }
        return value;
    }
    template <class T> void set_value(T value) {
        int ret = gp_widget_set_value(widget, value);
        if (ret != GP_OK) {
            throw GphotoError("gp_widget_set_value", ret);
        }
    }
    void get_range(float *min, float *max, float *increment);
    void write_to_camera(GphotoCamera &camera);
    void read_from_camera(GphotoCamera &camera);

    class children;
    children get_children();

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
        friend class children;
    };

    class children {
      public:
        child_iterator begin();
        child_iterator end();

      private:
        children(CameraWidget *widget);
        CameraWidget *widget;
        friend class GphotoCameraWidget;
    };

    class choices;
    choices get_choices();

    class choice_iterator
        : public std::iterator<std::input_iterator_tag, const char *> {
      public:
        choice_iterator &operator++();
        bool operator==(const choice_iterator &rhs) const;
        bool operator!=(const choice_iterator &rhs) const;
        const char *operator*();

      private:
        choice_iterator(CameraWidget *widget, int n);
        CameraWidget *widget;
        int n;
        friend class choices;
    };

    class choices {
      public:
        choice_iterator begin();
        choice_iterator end();

      private:
        choices(CameraWidget *widget);
        CameraWidget *widget;
        friend class GphotoCameraWidget;
    };

  private:
    GphotoCameraWidget(CameraWidget *widget);
    CameraWidget *widget;
    friend class GphotoCamera;
};

#endif
