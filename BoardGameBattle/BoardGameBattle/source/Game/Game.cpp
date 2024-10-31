#include "Game.h"

#include "../WindowManager/WindowManager.h"
#include "../AssetManager/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../InputManager/InputManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"
#include "../Entity/Button/Button.h"
#include "../Entity/SoundButton/SoundButton.h"

#include <iostream>
#include <memory>

Game::Game()
	: status(Game::Status::IN_MAIN_MENU)
	, soundEnabled(true)
	, mode(Game::Mode::NONE), color(Game::Color::NONE), multiplayerStatus(Game::MultiplayerStatus::NONE)
{
	// IN_MAIN_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_MAIN_MENU,

			VisualInterface(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"mainMenuBackgroundTexture"
				)
				, true
			)
		}
	);
	const auto& mainMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MAIN_MENU);
	mainMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Play",
			"buttonTexture",
			Game::Status::IN_MODE_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);
	mainMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.1f * WindowManager::get().getWindowHeight(),
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Settings",
			"buttonTexture",
			Game::Status::IN_SETTINGS_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);
	mainMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Exit",
			"buttonTexture",
			Game::Status::EXITING,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);

	// IN_SETTINGS_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_SETTINGS_MENU,

			VisualInterface(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"settingsMenuBackgroundTexture"
				)
				, true
			)
		}
	);
	const auto& settingsMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_SETTINGS_MENU);
	settingsMenuVisualInterface->second.addEntity
	(
		std::make_shared<SoundButton>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(0.0f, 1.0f, 0.0f),
			"arialFont",
			"Sound: ON",
			Button
			(
				WindowManager::get().getWindowWidth() / 2.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.3f * WindowManager::get().getWindowHeight(),
				0.25f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"Sound: ON",
				"buttonTexture",
				Game::Status::IN_SETTINGS_MENU,
				"buttonPressedSound",
				"buttonHoveredTexture",
				glm::vec3(0.75f, 0.75f, 0.75f),
				"buttonHoveredSound",
				Game::Mode::NONE,
				Game::Color::NONE,
				Game::MultiplayerStatus::NONE
			),
			Button
			(
				WindowManager::get().getWindowWidth() / 2.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
				0.25f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"Sound: OFF",
				"buttonTexture",
				Game::Status::IN_SETTINGS_MENU,
				"buttonPressedSound",
				"buttonHoveredTexture",
				glm::vec3(0.75f, 0.75f, 0.75f),
				"buttonHoveredSound",
				Game::Mode::NONE,
				Game::Color::NONE,
				Game::MultiplayerStatus::NONE
			)
		)
	);
	settingsMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Back",
			"buttonTexture",
			Game::Status::IN_MAIN_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);

	// IN_MODE_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_MODE_MENU,

			VisualInterface(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"modeMenuBackgroundTexture"
				)
				, true
			)
		}
	);
	const auto& modeMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MODE_MENU);
	modeMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Singleplayer",
			"buttonTexture",
			Game::Status::IN_SINGLEPLAYER_COLOR_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::SINGLEPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);
	modeMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Multiplayer",
			"buttonTexture",
			Game::Status::IN_MULTIPLAYER_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);
	modeMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Back",
			"buttonTexture",
			Game::Status::IN_MAIN_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);
	
	// IN_MULTIPLATER_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_MULTIPLAYER_MENU,

			VisualInterface(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"multiplayerMenuBackgroundTexture"
				)
				, true
			)
		}
	);
	const auto& multiplayerMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MULTIPLAYER_MENU);
	multiplayerMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Create Game",
			"buttonTexture",
			Game::Status::IN_INPUT_CREATE_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::CREATE_GAME
		)
	);
	multiplayerMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Join Game",
			"buttonTexture",
			Game::Status::IN_INPUT_JOIN_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::JOIN_GAME
		)
	);
	multiplayerMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Back",
			"buttonTexture",
			Game::Status::IN_MODE_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);

	// IN_SINGLEPLAYER_COLOR_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_SINGLEPLAYER_COLOR_MENU,

			VisualInterface(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"singleplayerColorMenuBackgroundTexture"
				)
				, true
			)
		}
	);
	const auto& singleplayerColorMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_SINGLEPLAYER_COLOR_MENU);
	singleplayerColorMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"White",
			"buttonTexture",
			Game::Status::IN_SINGLEPLAYER_GAME,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::SINGLEPLAYER,
			Game::Color::WHITE,
			Game::MultiplayerStatus::NONE
		)
	);
	singleplayerColorMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Black",
			"buttonTexture",
			Game::Status::IN_SINGLEPLAYER_GAME,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::SINGLEPLAYER,
			Game::Color::BLACK,
			Game::MultiplayerStatus::NONE
		)
	);
	singleplayerColorMenuVisualInterface->second.addEntity
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Back",
			"buttonTexture",
			Game::Status::IN_MODE_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::NONE,
			Game::Color::NONE,
			Game::MultiplayerStatus::NONE
		)
	);

	// IN_INPUT_CREATE_MENU

	// IM_INPUT_JOIN_MENU

	// IN_MULTIPLAYER_COLOR_MENU

	// IN_SINGLEPLAYER_GAME

	// IN_MULTIPLAYER_GAME
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
	WindowManager::get(); // Asigura setup-ul la OpenGL, trebuie sa fie primul apelat.
	AssetManager::get().loadResources();
}

void Game::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

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