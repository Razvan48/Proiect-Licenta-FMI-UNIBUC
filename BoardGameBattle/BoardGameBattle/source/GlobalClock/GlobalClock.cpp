#include "GlobalClock.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GlobalClock::GlobalClock()
{

}

GlobalClock::~GlobalClock()
{

}

GlobalClock& GlobalClock::get()
{
	static GlobalClock instance;
	return instance;
}

