#include "gphoto2cpp-internal.h"

GphotoCameraFile::GphotoCameraFile() {
    gpCall(gp_file_new, &file);
}

GphotoCameraFile::GphotoCameraFile(const GphotoCameraFile &rhs)
    : file(rhs.file) {
    gp_file_ref(file);
}

GphotoCameraFile::~GphotoCameraFile() {
    gp_file_unref(file);
}

void GphotoCameraFile::get_data_and_size(const char **data,
                                         unsigned long int *size) {
    gpCall(gp_file_get_data_and_size, file, data, size);
}
