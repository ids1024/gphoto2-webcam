#include "GphotoCameraFile.h"
#include "gpCall.h"

inline CameraFile *file_new() {
    CameraFile *file;
    gpCall(gp_file_new, &file);
    return file;
}

GphotoCameraFile::GphotoCameraFile() : file(file_new()) {}

void GphotoCameraFile::get_data_and_size(const char **data,
                                         unsigned long int *size) {
    gpCall(gp_file_get_data_and_size, file.get(), data, size);
}
