#include "GphotoContext.h"

GphotoContext::GphotoContext() {
    ctx = gp_context_new();
}

GphotoContext::GphotoContext(const GphotoContext &rhs) : ctx(rhs.ctx) {
    gp_context_ref(ctx);
}

GphotoContext::~GphotoContext() {
    gp_context_unref(ctx);
}

