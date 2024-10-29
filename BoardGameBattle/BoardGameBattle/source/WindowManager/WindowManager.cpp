#include "WindowManager.h"

WindowManager::WindowManager()
	: WINDOW_WIDTH(1024), WINDOW_HEIGHT(1024), WINDOW_TITLE("BoardGameBattle")
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Blocheaza redimensionarea ferestrei

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->WINDOW_TITLE.c_str(), 0, 0);
	// glfwGetPrimaryMonitor(); // pentru fullscreen

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