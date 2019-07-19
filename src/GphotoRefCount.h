#ifndef GPHOTO_REF_COUNT_H
#define GPHOTO_REF_COUNT_H

// A smart pointer for wrapping the style of ref counting used in gphoto2
template <typename T, auto &Ref, auto &Unref>
class GphotoRefCount {
  public:
    GphotoRefCount() = delete;
    inline GphotoRefCount(T* inner) : inner(inner) {};
    inline GphotoRefCount(const GphotoRefCount &rhs) : inner(rhs.inner) {
        Ref(inner);
    }
    GphotoRefCount(GphotoRefCount &&rhs) = default;
    inline ~GphotoRefCount() {
        Unref(inner);
    }
    inline GphotoRefCount &operator=(const GphotoRefCount &rhs) {
        if (this != &rhs) {
            Unref(inner);
            inner = rhs.inner;
            Ref(inner);
        }
        return *this;
    }
    inline GphotoRefCount &operator=(GphotoRefCount &&rhs) {
        if (this != &rhs) {
            Unref(inner);
            inner = rhs.inner;
        }
        return *this;
    }
    inline T* get() const {
        return inner;
    }
  private:
    T* inner;
};

#endif
