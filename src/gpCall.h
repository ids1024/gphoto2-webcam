#ifndef GPCALL_H
#define GPCALL_H

#include "GphotoError.h"

template <typename F, typename... Args>
static void _gpCall(const char *name, F f, Args... args) {
    int ret = f(args...);
    if (ret != GP_OK) {
        throw GphotoError(name, ret);
    }
}
#define gpCall(f, ...) ((_gpCall(#f, ((f)), __VA_ARGS__))) // NOLINT

#endif
