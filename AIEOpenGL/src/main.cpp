//#include "TestApplication.h"
#include "GeometryApp.h"
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
	
	BaseApplication* app = new GeometryApp();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}