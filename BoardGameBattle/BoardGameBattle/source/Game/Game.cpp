#include "Game.h"

#include "../WindowManager/WindowManager.h"
#include "../AssetManager/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../InputManager/InputManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"
#include "../Entity/Button/Button.h"

#include <iostream>
#include <memory>

Game::Game(Game::Status status, bool soundEnabled)
	: status(status), soundEnabled(soundEnabled)
{
	this->visualInterfaces.insert(
		{ 
			Game::Status::IN_MAIN_MENU,

			VisualInterface(TexturableEntity(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 2.0f,

			1.0f * WindowManager::get().getWindowWidth(),
			1.0f * WindowManager::get().getWindowHeight(),

			0.0f,
			false,
			false,
			"backgroundTexture",
			glm::vec3(245.0f / 255.0f, 245.0f / 255.0f, 220.0f / 255.0f),
			1.0f,
			1.0f
			), true)
		}
	);
	const auto& mainMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MAIN_MENU);
	mainMenuVisualInterface->second.addEntity(
		std::make_shared<Button>(
			//Button(
				WindowManager::get().getWindowWidth() / 2.0f,
				WindowManager::get().getWindowHeight() / 2.0f,
				0.25f * WindowManager::get().getWindowWidth(),
				0.25f * WindowManager::get().getWindowHeight(),
				0.0f,
				false,
				false,
				"buttonTexture",
				glm::vec3(1.0f, 1.0f, 1.0f),
				0.0f,
				1.0f,
				"arialFont",
				"Exit",
				Button::Status::RELEASED,
				Game::Status::EXITING,
				"buttonPressedSound"
			//)
		)
	);
}

Game::~Game()
{

}

Game& Game::get()
{
	static Game instance(Game::Status::IN_MAIN_MENU, true);
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

		// TEST:
		// Renderer::get().draw(500.0f, 500.0f, 100.0f, 100.0f, 25.0f, "whiteBishopTexture", glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f, false);
		// Renderer::get().drawText(500.0f, 500.0f, 500.0f, 10.0f * ((int)glfwGetTime() % 365), "arialFont", "Hello, world!", glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 0.5f);

		this->draw();
		this->update();

		glfwSwapBuffers(WindowManager::get().getWindow());
		glfwPollEvents();
	}
}

void Game::draw()
{
	const auto& visualInterface = this->visualInterfaces.find(this->status);
	if (visualInterface != this->visualInterfaces.end())
	{
		visualInterface->second.draw();
	}
	else
	{
		std::cout << "Error: Game Status " << (int)visualInterface->first << " requested for drawing not found in Visual Interfaces Map" << std::endl;
	}
}

void Game::update()
{
	const auto& visualInterface = this->visualInterfaces.find(this->status);
	if (visualInterface != this->visualInterfaces.end())
	{
		visualInterface->second.update();
	}
	else
	{
		std::cout << "Error: Game Status " << (int)visualInterface->first << " requested for updating not found in Visual Interfaces Map" << std::endl;
	}

	InputManager::get().update(); // Trebuie sa fie ultimul update, deoarece curata ce butoane s-au apasat.

	if (this->status == Game::Status::EXITING)
	{
		glfwSetWindowShouldClose(WindowManager::get().getWindow(), GLFW_TRUE);
	}
}

void Game::start()
{
	this->loadResources();
	this->run();
}