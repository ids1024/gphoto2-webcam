#include <gphoto2/gphoto2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <turbojpeg.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stropts.h>
#include <string.h>

#define WIDTH 960
#define HEIGHT 640

int main() {
	int ret;

	GPContext *ctx = gp_context_new();
	CameraList *list;

	Camera *camera;
	ret = gp_camera_new(&camera);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_new: %d\n", ret);
		return 1;
	}

	/*
	gp_list_new(&list);
	ret = gp_camera_autodetect(list, ctx);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_autodetect: %d\n", ret);
		return 1;
	}
	gp_list_free(list);
	if (gp_list_count(list) != 0) {
	}
	*/

	ret = gp_camera_init(camera, ctx);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_init: %d\n", ret);
		return 1;
	}

	int fd = open("/dev/video0", O_RDWR);
	if (fd == -1) {
		perror("fopen");
		return 1;
	}

	struct v4l2_format vid_format;
	memset(&vid_format, 0, sizeof(vid_format));
	ret = ioctl(fd, VIDIOC_G_FMT, &vid_format);
	/*
	if (ret == -1) {
		fprintf(stderr, "A\n");
		perror("ioctl");
		return 1;
	}
	*/

	vid_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vid_format.fmt.pix.width = WIDTH;
	vid_format.fmt.pix.height = HEIGHT;
	vid_format.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
	vid_format.fmt.pix.sizeimage = HEIGHT * WIDTH * 3;
	vid_format.fmt.pix.field = V4L2_FIELD_NONE;
	vid_format.fmt.pix.bytesperline = WIDTH * 3;
	vid_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;

	ret = ioctl(fd, VIDIOC_S_FMT, &vid_format);
	if (ret == -1) {
		perror("ioctl");
		return 1;
	}

	for (;;) {
		CameraFile *file;
		ret = gp_file_new(&file);
		if (ret != GP_OK) {
			fprintf(stderr, "gp_file_new: %d\n", ret);
			return 1;
		}
		gp_camera_capture_preview(camera, file, ctx);

		const char *data;
		unsigned long int size;
		ret = gp_file_get_data_and_size(file, &data, &size);
		if (ret != GP_OK) {
			fprintf(stderr, "gp_camera_capture_preview: %d\n", ret);
			return 1;

		}
		printf("%d\n", size);

		tjhandle handle = tjInitDecompress();
		unsigned char dstBuf[WIDTH * HEIGHT * 3];
		if (tjDecompress2(handle, data, size, dstBuf, WIDTH, 0, HEIGHT, TJPF_RGB, TJFLAG_ACCURATEDCT) != 0) {
			fprintf(stderr, "%s\n", tjGetErrorStr2(handle));
			return 1;
		}

		write(fd, dstBuf, WIDTH * HEIGHT * 3);
	}
}
