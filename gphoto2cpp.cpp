#include <string>

#include "gphoto2cpp.h"

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

