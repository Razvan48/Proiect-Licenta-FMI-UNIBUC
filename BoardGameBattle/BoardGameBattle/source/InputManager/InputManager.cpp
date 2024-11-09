#include "InputManager.h"

#include <iostream>

#include "../WindowManager/WindowManager.h"

InputManager::InputManager()
	: cursorPosX(0.0), cursorPosY(0.0)
	, cursorLastPressedLeftMousePosX(0.0), cursorLastPressedLeftMousePosY(0.0)
	, cursorLastPressedRightMousePosX(0.0), cursorLastPressedRightMousePosY(0.0)
	, leftMouseButtonPressed(false), rightMouseButtonPressed(false)
	, leftMouseButtonReleased(false), rightMouseButtonReleased(false)
	, leftMouseButtonHeld(false), rightMouseButtonHeld(false)
{
	this->pressedKeys.resize(GLFW_KEY_LAST, false);
	this->releasedKeys.resize(GLFW_KEY_LAST, false);
	this->heldKeys.resize(GLFW_KEY_LAST, false);

	glfwSetKeyCallback(WindowManager::get().getWindow(), InputManager::keyCallbackWrapper);
	glfwSetMouseButtonCallback(WindowManager::get().getWindow(), InputManager::mouseButtonCallbackWrapper);
	glfwSetCursorPosCallback(WindowManager::get().getWindow(), InputManager::cursorPositionCallbackWrapper);
}

InputManager::~InputManager()
{

}

InputManager& InputManager::get()
{
	static InputManager instance;
	return instance;
}

void InputManager::update()
{
	for (int i = 0; i < GLFW_KEY_LAST; ++i)
	{
		this->pressedKeys[i] = false;
		this->releasedKeys[i] = false;
	}

	this->leftMouseButtonPressed = false;
	this->rightMouseButtonPressed = false;
	this->leftMouseButtonReleased = false;
	this->rightMouseButtonReleased = false;
}

void InputManager::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager::get().keyCallback(window, key, scancode, action, mods);
}

void InputManager::mouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
{
	InputManager::get().mouseButtonCallback(window, button, action, mods);
}

void InputManager::cursorPositionCallbackWrapper(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::get().cursorPositionCallback(window, xpos, ypos);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		this->pressedKeys[key] = true;
		this->heldKeys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		this->releasedKeys[key] = true;
		this->heldKeys[key] = false;
	}
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			this->leftMouseButtonPressed = true;
			this->leftMouseButtonHeld = true;

			this->cursorLastPressedLeftMousePosX = this->cursorPosX;
			this->cursorLastPressedLeftMousePosY = this->cursorPosY;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			this->rightMouseButtonPressed = true;
			this->rightMouseButtonHeld = true;

			this->cursorLastPressedRightMousePosX = this->cursorPosX;
			this->cursorLastPressedRightMousePosY = this->cursorPosY;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			this->leftMouseButtonReleased = true;
			this->leftMouseButtonHeld = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			this->rightMouseButtonReleased = true;
			this->rightMouseButtonHeld = false;
		}
	}
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	this->cursorPosX = xpos;
	this->cursorPosY = ypos;
}