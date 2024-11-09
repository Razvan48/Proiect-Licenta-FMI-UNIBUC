#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GlobalClock
{
private:
	GlobalClock();
	~GlobalClock();
	GlobalClock(const GlobalClock& other) = delete;
	GlobalClock& operator= (const GlobalClock& other) = delete;
	GlobalClock(const GlobalClock&& other) = delete;
	GlobalClock& operator= (const GlobalClock&& other) = delete;

public:
	static GlobalClock& get();

	inline double getCurrentTime() const { return glfwGetTime(); }
};