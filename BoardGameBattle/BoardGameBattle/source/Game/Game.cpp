#include "Game.h"

#include "../WindowManager/WindowManager.h"

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

#include "../Renderer/Renderer.h" // TODO: de sters, doar test

void Game::loadResources()
{
	WindowManager::get();
	Renderer::get(); // TODO: de sters, doar test
}

void Game::run()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		this->update();

		glfwSwapBuffers(WindowManager::get().getWindow());
		glfwPollEvents();
	}
}

void Game::start()
{
	this->loadResources();
	this->run();
}

void Game::update()
{

}