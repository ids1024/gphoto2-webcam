#ifndef GPHOTO2CPP_INTERNAL_H
#define GPHOTO2CPP_INTERNAL_H

#include "gphoto2cpp.h"

template <typename F, typename... Args>
static void _gpCall(const char *name, F f, Args... args) {
    int ret = f(args...);
    if (ret != GP_OK) {
        throw Gphoto2Error(name, ret);
    }
}
#define gpCall(f, ...) ((_gpCall(#f, ((f)), __VA_ARGS__)))

#endif
