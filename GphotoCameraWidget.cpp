#include "gpCall.h"

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

bool GphotoCameraWidget::get_readonly() {
    int readonly;
    gpCall(gp_widget_get_readonly, widget, &readonly);
    return readonly;
}

GphotoCameraWidget::Children GphotoCameraWidget::children() {
    return Children(widget);
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

GphotoCameraWidget::Children::Children(CameraWidget *widget) : widget(widget){};

GphotoCameraWidget::child_iterator GphotoCameraWidget::Children::begin() {
    return GphotoCameraWidget::child_iterator(widget, 0);
};

GphotoCameraWidget::child_iterator GphotoCameraWidget::Children::end() {
    int count = gp_widget_count_children(widget);
    return GphotoCameraWidget::child_iterator(widget, count);
};

GphotoCameraWidget::GphotoCameraWidget(CameraWidget *widget) : widget(widget){};


