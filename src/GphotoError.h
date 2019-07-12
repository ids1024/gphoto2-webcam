#ifndef GPHOTO_ERROR_H
#define GPHOTO_ERROR_H

#include <exception>
#include <string>

#include <gphoto2/gphoto2.h>

class GphotoError : public std::exception {
  private:
    std::string message;

  public:
    inline GphotoError(std::string func, int err) {
        message = func + ": " + std::string(gp_result_as_string(err));
    }

    inline const char *what() const throw() {
        return message.c_str();
    }
};

#endif
