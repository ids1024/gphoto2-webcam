#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <system_error>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "V4l2Loopback.h"

V4l2Loopback::V4l2Loopback(const char *path) {
	fd = open(path, O_WRONLY);
	if (fd == -1) {
		throw std::system_error(errno, std::generic_category(), "open");
	}
}

void V4l2Loopback::set_vidformat(int height, int width) {
	struct v4l2_format vid_format;
	memset(&vid_format, 0, sizeof(vid_format));

	vid_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vid_format.fmt.pix.width = width;
	vid_format.fmt.pix.height = height;
	vid_format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	vid_format.fmt.pix.field = V4L2_FIELD_NONE;
	vid_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;

	int ret = ioctl(fd, VIDIOC_S_FMT, &vid_format);
	if (ret == -1) {
		throw std::system_error(errno, std::generic_category(), "ioctl");
	}
}

void V4l2Loopback::write_frame(const void *frame, size_t size) {
	int ret = write(fd, frame, size);
	if (ret == -1) {
		throw std::system_error(errno, std::generic_category(), "write");
	}
}
