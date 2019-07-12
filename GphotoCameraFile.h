#ifndef GPHOTO_CAMERA_FILE_H
#define GPHOTO_CAMERA_FILE_H

#include <gphoto2/gphoto2.h>

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

#endif
