#include <gphoto2/gphoto2.h>
#include <stdio.h>
#include <exception>
#include <string>

#include "V4l2Loopback.h"

#define WIDTH 960
#define HEIGHT 640

class Gphoto2Error: public std::exception {
	std::string func;
	int err;
	public:
	Gphoto2Error(std::string func, int err) {
		this->func = func;
		this->err = err;
	}
	const char* what() const throw() {
		return (func + ": " + std::to_string(err)).c_str();
	}
};

void capture_preview(Camera* camera, CameraFile **file, const char **data, unsigned long int *size, GPContext *ctx) {
	int ret;

	ret = gp_file_new(file);
	if (ret != GP_OK) {
		throw Gphoto2Error("gp_file_new", ret);
	}
	gp_camera_capture_preview(camera, *file, ctx);

	ret = gp_file_get_data_and_size(*file, data, size);
	if (ret != GP_OK) {
		throw Gphoto2Error("gp_camera_capture_preview", ret);
	}
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

	V4l2Loopback loopback("/dev/video0");
	loopback.set_vidformat(HEIGHT, WIDTH);

	for (;;) {
		CameraFile *file;
		const char *data;
		unsigned long int size;
		capture_preview(camera, &file, &data, &size, ctx);
		loopback.write_frame(data, size);
		gp_file_unref(file);
	}
}
