//#include "TestApplication.h"
#include "GeometryApp.h"
#include <GLFW/glfw3.h>

int main() {
	
	BaseApplication* app = new GeometryApp();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}