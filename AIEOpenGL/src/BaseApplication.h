#pragma once

struct GLFWwindow;

#define Color_Red glm::vec4(1,0,0,1)
#define Color_Yellow glm::vec4(1,1,0,1)
#define Color_Green glm::vec4(0,1,0,1)
#define Color_Blue glm::vec4(0,0,1,1)
#define Color_Orange glm::vec4(1,.5f,0,1)
#define Color_White glm::vec4(1,1,1,1)
#define Color_Black glm::vec4(0,0,0,1)
#define Color_Cyan glm::vec4(0,0.5f,0.5f,1)

class BaseApplication {
public:

	BaseApplication() {}
	virtual ~BaseApplication() {}

	void run();
	
	virtual bool startup() = 0;
	virtual void shutdown() = 0;

	virtual bool update(float deltaTime) = 0;
	virtual void draw() = 0;

protected:

	virtual bool createWindow(const char* title, int width, int height);
	virtual void destroyWindow();

	GLFWwindow*	m_window;
};