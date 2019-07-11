#ifndef GPHOTO2CPP_H
#define GPHOTO2CPP_H

#include <exception>
#include <gphoto2/gphoto2.h>
#include <iterator>
#include <string>

class Gphoto2Error : public std::exception {
  private:
    std::string message;

  public:
    inline Gphoto2Error(std::string func, int err) {
      message = func + ": " + std::string(gp_result_as_string(err));
    }

    inline const char *what() const throw() {
      return message.c_str();
    }
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

class GphotoCameraWidget {
  public:
    GphotoCameraWidget(const GphotoCameraWidget &rhs);
    ~GphotoCameraWidget();
    GphotoCameraWidget &operator=(const GphotoCameraWidget &) = delete;
    const char *get_name();
    bool get_readonly();

    class Children;
    Children children();

    class child_iterator
        : public std::iterator<std::input_iterator_tag, GphotoCameraWidget> {
      public:
        child_iterator &operator++();
        bool operator==(const child_iterator &rhs) const;
        bool operator!=(const child_iterator &rhs) const;
        GphotoCameraWidget operator*();

      private:
        child_iterator(CameraWidget *widget, int n);
        CameraWidget *widget;
        int n;
        friend class Children;
    };

    class Children {
      public:
        child_iterator begin();
        child_iterator end();

      private:
        Children(CameraWidget *widget);
        CameraWidget *widget;
        friend class GphotoCameraWidget;
    };

  private:
    GphotoCameraWidget(CameraWidget *widget);
    CameraWidget *widget;
    friend class GphotoCamera;
};

class GphotoCamera {
  public:
    GphotoCamera(GphotoContext ctx);
    GphotoCamera(const GphotoCamera &rhs);
    ~GphotoCamera();
    GphotoCamera &operator=(const GphotoCamera &) = delete;
    GphotoCameraWidget get_config();
    void init();
    GphotoCameraFile capture_preview();

  private:
    GphotoContext ctx;
    Camera *camera;
};

#endif
