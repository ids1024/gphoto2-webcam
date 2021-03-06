#include "GphotoCameraWidget.h"
#include "GphotoCamera.h"
#include "gpCall.h"

static std::string get_full_name(CameraWidget *widget) {
    CameraWidget *parent;
    const char *name;
    gp_widget_get_parent(widget, &parent);
    gp_widget_get_name(widget, &name);
    if (parent == NULL)
        return "/" + std::string(name);
    else
        return get_full_name(parent) + "/" + std::string(name);
}

static CameraWidget *widget_new(CameraWidgetType type, const char *label) {
    CameraWidget *widget;
    gpCall(gp_widget_new, type, label, &widget);
    return widget;
}

GphotoCameraWidget::GphotoCameraWidget(CameraWidgetType type, const char *label) : widget(widget_new(type, label)) {}

const char *GphotoCameraWidget::get_name() {
    const char *name;
    gpCall(gp_widget_get_name, widget.get(), &name);
    return name;
}

const char *GphotoCameraWidget::get_label() {
    const char *label;
    gpCall(gp_widget_get_label, widget.get(), &label);
    return label;
}

bool GphotoCameraWidget::get_readonly() {
    int readonly;
    gpCall(gp_widget_get_readonly, widget.get(), &readonly);
    return readonly;
}

CameraWidgetType GphotoCameraWidget::get_type() {
    CameraWidgetType type;
    gpCall(gp_widget_get_type, widget.get(), &type);
    return type;
}

void GphotoCameraWidget::get_range(float *min, float *max, float *increment) {
    gpCall(gp_widget_get_range, widget.get(), min, max, increment);
}

void GphotoCameraWidget::write_to_camera(GphotoCamera &camera) {
    std::string name = get_full_name(widget.get());
    gpCall(gp_camera_set_single_config, camera.camera.get(), name.c_str(), widget.get(), camera.ctx.ctx.get());
}

void GphotoCameraWidget::read_from_camera(GphotoCamera &camera) {
    std::string name = get_full_name(widget.get());
    CameraWidget *new_widget;
    gpCall(gp_camera_get_single_config, camera.camera.get(), name.c_str(), &new_widget, camera.ctx.ctx.get());
    widget = new_widget;
}

GphotoCameraWidget::children GphotoCameraWidget::get_children() {
    return children(widget.get());
}

GphotoCameraWidget::choices GphotoCameraWidget::get_choices() {
    return choices(widget.get());
}

GphotoCameraWidget::GphotoCameraWidget(CameraWidget *widget) : widget(widget){};
