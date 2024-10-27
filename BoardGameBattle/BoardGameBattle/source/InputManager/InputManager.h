#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class InputManager
{
private:
	InputManager();
	~InputManager();
	InputManager(const InputManager& other) = delete;
	InputManager& operator= (const InputManager& other) = delete;
	InputManager(const InputManager&& other) = delete;
	InputManager& operator= (const InputManager&& other) = delete;

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

	static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallbackWrapper(GLFWwindow* window, double xpos, double ypos);

	std::vector<bool> pressedKeys;
	std::vector<bool> releasedKeys;
	std::vector<bool> heldKeys;

	double cursorPosX;
	double cursorPosY;

	bool leftMouseButtonPressed;
	bool rightMouseButtonPressed;
	bool leftMouseButtonReleased;
	bool rightMouseButtonReleased;
	bool leftMouseButtonHeld;
	bool rightMouseButtonHeld;

public:

	static InputManager& get();
	void update();

	inline double getCursorPosX() const { return this->cursorPosX; }
	inline double getCursorPosY() const { return this->cursorPosY; }
	inline bool isKeyPressed(int key) const { return this->pressedKeys[key]; }
	inline bool isKeyReleased(int key) const { return this->releasedKeys[key]; }
	inline bool isKeyHeld(int key) const { return this->heldKeys[key]; }
	inline bool isLeftMouseButtonPressed() const { return this->leftMouseButtonPressed; }
	inline bool isRightMouseButtonPressed() const { return this->rightMouseButtonPressed; }
	inline bool isLeftMouseButtonReleased() const { return this->leftMouseButtonReleased; }
	inline bool isRightMouseButtonReleased() const { return this->rightMouseButtonReleased; }
	inline bool isLeftMouseButtonHeld() const { return this->leftMouseButtonHeld; }
	inline bool isRightMouseButtonHeld() const { return this->rightMouseButtonHeld; }
};