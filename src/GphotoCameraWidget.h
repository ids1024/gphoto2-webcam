#ifndef GPHOTO_CAMERA_WIDGET_H
#define GPHOTO_CAMERA_WIDGET_H

#include <gphoto2/gphoto2.h>
#include <iterator>

#include "GphotoError.h"
#include "GphotoRefCount.h"
#include "GphotoBaseIterator.h"

class GphotoCamera;

class GphotoCameraWidget {
  public:
    GphotoCameraWidget(CameraWidgetType type, const char *label);
    template <class T> GphotoCameraWidget(CameraWidgetType type, const char *label, T value) : GphotoCameraWidget(type, label) {
        set_value(value);
    }
    const char *get_name();
    const char *get_label();
    bool get_readonly();
    CameraWidgetType get_type();
    template <class T> T get_value() {
        T value;
        int ret = gp_widget_get_value(widget.get(), &value);
        if (ret != GP_OK) {
            throw GphotoError("gp_widget_get_value", ret);
        }
        return value;
    }
    template <class T> void set_value(T value) {
        int ret = gp_widget_set_value(widget.get(), value);
        if (ret != GP_OK) {
            throw GphotoError("gp_widget_set_value", ret);
        }
    }
    void get_range(float *min, float *max, float *increment);
    void write_to_camera(GphotoCamera &camera);
    void read_from_camera(GphotoCamera &camera);

    class choice_iterator : public GphotoBaseIterator<CameraWidget, const char*> {
      public:
        using GphotoBaseIterator::GphotoBaseIterator;
        inline const char *operator*() {
            const char *choice;
            int ret = gp_widget_get_choice(obj, n, &choice);
            if (ret != GP_OK) {
                throw GphotoError("gp_widget_get_choice", ret);
            }
            return choice;
        }
    };

    class child_iterator : public GphotoBaseIterator<CameraWidget, GphotoCameraWidget> {
      public:
        using GphotoBaseIterator::GphotoBaseIterator;
        inline GphotoCameraWidget operator*() {
          CameraWidget *child;
          int ret = gp_widget_get_child(obj, n, &child);
            if (ret != GP_OK) {
                throw GphotoError("gp_widget_get_child", ret);
            }
          gp_widget_ref(child);
          return GphotoCameraWidget(child);
        }
    };

    class children : public GphotoBaseIterable<CameraWidget, child_iterator> {
        using GphotoBaseIterable::GphotoBaseIterable;
        inline int count() {
            return gp_widget_count_children(obj);
        }
        friend class GphotoCameraWidget;
    };

    class choices : public GphotoBaseIterable<CameraWidget, choice_iterator> {
        using GphotoBaseIterable::GphotoBaseIterable;
        inline int count() {
            return gp_widget_count_choices(obj);
        }
        friend class GphotoCameraWidget;
    };

    children get_children();
    choices get_choices();

  private:
    GphotoCameraWidget(CameraWidget *widget);
    GphotoRefCount<CameraWidget, gp_widget_ref, gp_widget_unref> widget;
    friend class GphotoCamera;
};

#endif
