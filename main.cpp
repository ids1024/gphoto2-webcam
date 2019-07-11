#include <gphoto2/gphoto2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stropts.h>
#include <string.h>

#define WIDTH 960
#define HEIGHT 640

int capture_preview(Camera* camera, CameraFile **file, const char **data, unsigned long int *size, GPContext *ctx) {
	// XXX correct size of buffer
	
	int ret;

	ret = gp_file_new(file);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_file_new: %d\n", ret);
		return 1;
	}
	gp_camera_capture_preview(camera, *file, ctx);

	ret = gp_file_get_data_and_size(*file, data, size);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_capture_preview: %d\n", ret);
		return 1;

	}

	return 0;
}

int set_vidformat(int fd) {
	int ret;

	struct v4l2_format vid_format;
	memset(&vid_format, 0, sizeof(vid_format));
	ret = ioctl(fd, VIDIOC_G_FMT, &vid_format);

	vid_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vid_format.fmt.pix.width = WIDTH;
	vid_format.fmt.pix.height = HEIGHT;
	vid_format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	vid_format.fmt.pix.field = V4L2_FIELD_NONE;
	vid_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;

	ret = ioctl(fd, VIDIOC_S_FMT, &vid_format);
	if (ret == -1) {
		perror("ioctl");
		return 1;
	}

	return 0;
}

int main() {
	int ret;

	GPContext *ctx = gp_context_new();

	Camera *camera;
	ret = gp_camera_new(&camera);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_new: %d\n", ret);
		return 1;
	}

	ret = gp_camera_init(camera, ctx);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_init: %d\n", ret);
		return 1;
	}

 	CameraWidget *window;
	ret = gp_camera_get_config(camera, &window, ctx);
	if (ret != GP_OK) {
		fprintf(stderr, "gp_camera_get_config: %d\n", ret);
		return 1;
	}
	for (int i = 0; i < gp_widget_count_children(window); i++) {
		CameraWidget *child;
		ret = gp_widget_get_child(window, i, &child);
		if (ret != GP_OK) {
			fprintf(stderr, "gp_widget_get_child: %d\n", ret);
			return 1;
		}
		const char *name;
		gp_widget_get_name(child, &name);
		printf("%s\n", name);
	}

	int fd = open("/dev/video0", O_WRONLY);
	if (fd == -1) {
		perror("open");
		return 1;
	}

	ret = set_vidformat(fd);
	if (ret != 0) {
		fprintf(stderr, "set_vidformat: %d\n", ret);
		return 1;
	}

	for (;;) {
		CameraFile *file;
		const char *data;
		unsigned long int size;
		ret = capture_preview(camera, &file, &data, &size, ctx);
		if (ret != 0) {
			fprintf(stderr, "capture_preview: %d\n", ret);
			return 1;
		}
		write(fd, data, size);
		gp_file_unref(file);
	}
}