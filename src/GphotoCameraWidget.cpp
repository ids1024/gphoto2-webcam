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

GphotoCameraWidget::GphotoCameraWidget(const GphotoCameraWidget &rhs)
    : widget(rhs.widget) {
    gp_widget_ref(widget);
}

GphotoCameraWidget::~GphotoCameraWidget() {
    gp_widget_unref(widget);
}

const char *GphotoCameraWidget::get_name() {
    const char *name;
    gpCall(gp_widget_get_name, widget, &name);
    return name;
}

const char *GphotoCameraWidget::get_label() {
    const char *label;
    gpCall(gp_widget_get_label, widget, &label);
    return label;
}

bool GphotoCameraWidget::get_readonly() {
    int readonly;
    gpCall(gp_widget_get_readonly, widget, &readonly);
    return readonly;
}

CameraWidgetType GphotoCameraWidget::get_type() {
    CameraWidgetType type;
    gpCall(gp_widget_get_type, widget, &type);
    return type;
}

void GphotoCameraWidget::get_range(float *min, float *max, float *increment) {
    gpCall(gp_widget_get_range, widget, min, max, increment);
}

void GphotoCameraWidget::write_to_camera(GphotoCamera &camera) {
    std::string name = get_full_name(widget);
    gpCall(gp_camera_set_single_config, camera.camera, name.c_str(), widget, camera.ctx.ctx);
}

void GphotoCameraWidget::read_from_camera(GphotoCamera &camera) {
    std::string name = get_full_name(widget);
    CameraWidget *new_widget;
    gpCall(gp_camera_get_single_config, camera.camera, name.c_str(), &new_widget, camera.ctx.ctx);
    widget = new_widget;
}

GphotoCameraWidget::children GphotoCameraWidget::get_children() {
    return children(widget);
}

GphotoCameraWidget::child_iterator &GphotoCameraWidget::child_iterator::
operator++() {
    n++;
    return *this;
}
bool GphotoCameraWidget::child_iterator::
operator==(const child_iterator &rhs) const {
    return n == rhs.n;
}

bool GphotoCameraWidget::child_iterator::
operator!=(const child_iterator &rhs) const {
    return n != rhs.n;
}

GphotoCameraWidget GphotoCameraWidget::child_iterator::operator*() {
    CameraWidget *child;
    gpCall(gp_widget_get_child, widget, n, &child);
    gp_widget_ref(child);
    return GphotoCameraWidget(child);
}

GphotoCameraWidget::child_iterator::child_iterator(CameraWidget *widget, int n)
    : widget(widget), n(n) {
}

GphotoCameraWidget::children::children(CameraWidget *widget) : widget(widget){};

GphotoCameraWidget::child_iterator GphotoCameraWidget::children::begin() {
    return GphotoCameraWidget::child_iterator(widget, 0);
};

GphotoCameraWidget::child_iterator GphotoCameraWidget::children::end() {
    int count = gp_widget_count_children(widget);
    return GphotoCameraWidget::child_iterator(widget, count);
};

GphotoCameraWidget::choices GphotoCameraWidget::get_choices() {
    return choices(widget);
}

GphotoCameraWidget::choice_iterator &GphotoCameraWidget::choice_iterator::
operator++() {
    n++;
    return *this;
}
bool GphotoCameraWidget::choice_iterator::
operator==(const choice_iterator &rhs) const {
    return n == rhs.n;
}

bool GphotoCameraWidget::choice_iterator::
operator!=(const choice_iterator &rhs) const {
    return n != rhs.n;
}

const char *GphotoCameraWidget::choice_iterator::operator*() {
    const char *choice;
    gpCall(gp_widget_get_choice, widget, n, &choice);
    return choice;
}

GphotoCameraWidget::choice_iterator::choice_iterator(CameraWidget *widget,
                                                     int n)
    : widget(widget), n(n) {
}

GphotoCameraWidget::choices::choices(CameraWidget *widget) : widget(widget){};

GphotoCameraWidget::choice_iterator GphotoCameraWidget::choices::begin() {
    return GphotoCameraWidget::choice_iterator(widget, 0);
};

GphotoCameraWidget::choice_iterator GphotoCameraWidget::choices::end() {
    int count = gp_widget_count_choices(widget);
    return GphotoCameraWidget::choice_iterator(widget, count);
}

GphotoCameraWidget::GphotoCameraWidget(CameraWidget *widget) : widget(widget){};
