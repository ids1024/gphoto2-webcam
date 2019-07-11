#include <string>

#include "gphoto2cpp.h"

template <typename F, typename... Args>
static void _gpCall(const char *name, F f, Args... args) {
    int ret = f(args...);
    if (ret != GP_OK) {
        throw Gphoto2Error(name, ret);
    }
}
#define gpCall(f, ...) ((_gpCall(#f, ((f)), __VA_ARGS__)))

Gphoto2Error::Gphoto2Error(std::string func, int err) {
    message = func + ": " + std::string(gp_result_as_string(err));
}
const char *Gphoto2Error::what() const throw() {
    return message.c_str();
}

GphotoContext::GphotoContext() {
    ctx = gp_context_new();
}

GphotoContext::GphotoContext(const GphotoContext &rhs) : ctx(rhs.ctx) {
    gp_context_ref(ctx);
}

GphotoContext::~GphotoContext() {
    gp_context_unref(ctx);
}

GphotoCameraFile::GphotoCameraFile() {
    gpCall(gp_file_new, &file);
}

GphotoCameraFile::GphotoCameraFile(const GphotoCameraFile &rhs)
    : file(rhs.file) {
    gp_file_ref(file);
}

GphotoCameraFile::~GphotoCameraFile() {
    gp_file_unref(file);
}

void GphotoCameraFile::get_data_and_size(const char **data,
                                         unsigned long int *size) {
    gpCall(gp_file_get_data_and_size, file, data, size);
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

GphotoCamera::GphotoCamera(GphotoContext ctx) : ctx(ctx) {
    gpCall(gp_camera_new, &camera);
}

GphotoCamera::GphotoCamera(const GphotoCamera &rhs)
    : ctx(rhs.ctx), camera(rhs.camera) {
    gp_camera_ref(camera);
}

GphotoCamera::~GphotoCamera() {
    gp_camera_unref(camera);
}
GphotoCameraWidget GphotoCamera::get_config() {
    CameraWidget *window;
    gpCall(gp_camera_get_config, camera, &window, ctx.ctx);
    return GphotoCameraWidget(window);
}

void GphotoCamera::init() {
    gpCall(gp_camera_init, camera, ctx.ctx);
}

GphotoCameraFile GphotoCamera::capture_preview() {
    GphotoCameraFile file;
    gp_camera_capture_preview(camera, file.file, ctx.ctx);
    return file;
}
