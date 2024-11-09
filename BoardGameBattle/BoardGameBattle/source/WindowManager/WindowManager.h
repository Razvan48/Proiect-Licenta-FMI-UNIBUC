#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class WindowManager
{
private:
	WindowManager();
	~WindowManager();
	WindowManager(const WindowManager& other) = delete;
	WindowManager& operator= (const WindowManager& other) = delete;
	WindowManager(const WindowManager&& other) = delete;
	WindowManager& operator= (const WindowManager&& other) = delete;

	GLFWwindow* window;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	const std::string WINDOW_TITLE;

public:
	static WindowManager& get();
	inline int getWindowWidth() const { return this->WINDOW_WIDTH; }
	inline int getWindowHeight() const { return this->WINDOW_HEIGHT; }
	inline GLFWwindow* getWindow() { return this->window; }
};