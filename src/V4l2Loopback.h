#ifndef V4L2LOOPBACK_H
#define V4L2LOOPBACK_H

class V4l2Loopback {
  public:
    V4l2Loopback(const char *path);
    void set_vidformat(int height, int width);
    void write_frame(const void *frame, size_t size);

  private:
    int fd;
};

#endif
