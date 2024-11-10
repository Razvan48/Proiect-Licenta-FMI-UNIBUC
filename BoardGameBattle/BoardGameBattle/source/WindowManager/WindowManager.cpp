#include "WindowManager.h"

WindowManager::WindowManager()
	: WINDOW_WIDTH(512), WINDOW_HEIGHT(512), WINDOW_TITLE("BoardGameBattle")
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Blocheaza redimensionarea ferestrei

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->WINDOW_TITLE.c_str(), 0, 0);

	// Urmatoarele 3 linii sunt pentru fullscreen mode.
	// this->window = glfwCreateWindow(videoMode->width, videoMode->height, this->WINDOW_TITLE.c_str(), primaryMonitor, 0);
	// this->WINDOW_WIDTH = videoMode->width;
	// this->WINDOW_HEIGHT = videoMode->height;

    glfwMakeContextCurrent(this->window);

    glewInit();
}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

WindowManager& WindowManager::get()
{
	static WindowManager instance;
	return instance;
}