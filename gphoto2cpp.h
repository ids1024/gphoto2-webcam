#ifndef GPHOTO2CPP_H
#define GPHOTO2CPP_H

#include <exception>
#include <gphoto2/gphoto2.h>
#include <string>

class Gphoto2Error : public std::exception {
  private:
    std::string message;

  public:
    Gphoto2Error(std::string func, int err);
    const char *what() const throw();
};

class GphotoContext {
  public:
    GphotoContext();
    GphotoContext(const GphotoContext &rhs);
    ~GphotoContext();
    GphotoContext &operator=(const GphotoContext &) = delete;

  private:
    GPContext *ctx;
    friend class GphotoCamera;
};

class GphotoCameraFile {
  public:
    GphotoCameraFile();
    GphotoCameraFile(const GphotoCameraFile &rhs);
    ~GphotoCameraFile();
    GphotoCameraFile &operator=(const GphotoCameraFile &) = delete;
    void get_data_and_size(const char **data, unsigned long int *size);
  private:
    CameraFile *file;
    friend class GphotoCamera;
};

class GphotoCamera {
  public:
    GphotoCamera(GphotoContext ctx);
    GphotoCamera(const GphotoCamera &rhs);
    ~GphotoCamera();
    GphotoCamera &operator=(const GphotoCamera &) = delete;
    CameraWidget *get_config();
    void init();
    GphotoCameraFile capture_preview();

  private:
    GphotoContext ctx;
    Camera *camera;
};

#endif
