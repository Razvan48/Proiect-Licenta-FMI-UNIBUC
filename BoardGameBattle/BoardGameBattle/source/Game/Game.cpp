#include "Game.h"

#include "../WindowManager/WindowManager.h"
#include "../AssetManager/AssetManager.h"
#include "../Renderer/Renderer.h"

Game::Game()
{

}

Game::~Game()
{

}

Game& Game::get()
{
	static Game instance;
	return instance;
}

void Game::loadResources()
{
	WindowManager::get(); // Asigura setup-ul la OpenGL
	AssetManager::get().loadResources();
}

void Game::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer::get().draw(500.0f, 500.0f, 100.0f, 100.0f, 25.0f, "test2", glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
		Renderer::get().drawText(500.0f, 500.0f, 300.0f, "arial", "Hello World", glm::vec3(1.0f, 0.0f, 1.0f), 1.0f);

		this->update();

		glfwSwapBuffers(WindowManager::get().getWindow());
		glfwPollEvents();
	}
}

void Game::update()
{

}

void Game::start()
{
	this->loadResources();
	this->run();
}