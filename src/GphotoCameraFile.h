#ifndef GPHOTO_CAMERA_FILE_H
#define GPHOTO_CAMERA_FILE_H

#include <gphoto2/gphoto2.h>

#include "GphotoRefCount.h"

class GphotoCameraFile {
  public:
    GphotoCameraFile();
    void get_data_and_size(const char **data, unsigned long int *size);
  private:
    GphotoRefCount<CameraFile, gp_file_ref, gp_file_unref> file;
    friend class GphotoCamera;
};

#endif
