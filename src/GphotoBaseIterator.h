#ifndef GPHOTO_ITERATOR_H
#define GPHOTO_ITERATOR_H

#include <iterator>

template <typename T, typename E>
class GphotoBaseIterator {
    using difference_type = int;
    using value_type = E;
    using pointer = E*;
    using reference = E&;
    using iterator_category = std::input_iterator_tag;
  public:
    inline GphotoBaseIterator &operator++() {
        n++;
        return *this;
    }
    inline bool operator==(const GphotoBaseIterator &rhs) const {
        return n == rhs.n;
    }
    inline bool operator!=(const GphotoBaseIterator &rhs) const {
        return n != rhs.n;
    }
    virtual E operator*() = 0;
    inline GphotoBaseIterator(T *obj, int n) : obj(obj), n(n) {};
  protected:
    T *obj;
    int n;
};

#endif
