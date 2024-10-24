#include "Game.h"

#include <iostream>

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
	WindowManager::get();
	Renderer::get();
	AssetManager::get().loadResources();
}

void Game::run()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer::get().draw(0.0f, 0.0f, 0.25f, 0.25f, 90.0f, "test", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);

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