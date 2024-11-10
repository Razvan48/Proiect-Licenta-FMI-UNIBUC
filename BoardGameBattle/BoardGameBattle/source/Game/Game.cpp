#include "Game.h"

#include "../WindowManager/WindowManager.h"
#include "../AssetManager/AssetManager.h"
#include "../Renderer/Renderer.h"
#include "../InputManager/InputManager.h"

#include "../Entity/TexturableEntity/TexturableEntity.h"
#include "../Entity/Button/Button.h"
#include "../Entity/SoundSetting/SoundSetting.h"
#include "../Entity/DataBox/DataBox.h"
#include "../Entity/CreateInputForm/CreateInputForm.h"
#include "../Entity/JoinInputForm/JoinInputForm.h"

#include "../BoardVisualizer/BoardVisualizer.h"

#include "../VisualInterface/SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"
#include "../VisualInterface/CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/JoinedMultiplayerGameVisualInterface/JoinedMultiplayerGameVisualInterface.h"
#include "../VisualInterface/MultiplayerColorMenuVisualInterface/MultiplayerColorMenuVisualInterface.h"

#include <enet/enet.h>

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

			std::make_shared<VisualInterface>(
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
	mainMenuVisualInterface->second->addEntity
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
	mainMenuVisualInterface->second->addEntity
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
	mainMenuVisualInterface->second->addEntity
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

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"settingsMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& settingsMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_SETTINGS_MENU);
	settingsMenuVisualInterface->second->addEntity
	(
		std::make_shared<SoundSetting>
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
	settingsMenuVisualInterface->second->addEntity
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

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"modeMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& modeMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MODE_MENU);
	modeMenuVisualInterface->second->addEntity
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
	modeMenuVisualInterface->second->addEntity
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
	modeMenuVisualInterface->second->addEntity
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

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"multiplayerMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& multiplayerMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_MULTIPLAYER_MENU);
	multiplayerMenuVisualInterface->second->addEntity
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
	multiplayerMenuVisualInterface->second->addEntity
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
	multiplayerMenuVisualInterface->second->addEntity
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

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"singleplayerColorMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& singleplayerColorMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_SINGLEPLAYER_COLOR_MENU);
	singleplayerColorMenuVisualInterface->second->addEntity
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
	singleplayerColorMenuVisualInterface->second->addEntity
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
	singleplayerColorMenuVisualInterface->second->addEntity
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
	this->visualInterfaces.insert(
		{
			Game::Status::IN_INPUT_CREATE_MENU,

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"inputCreateMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& inputCreateMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_INPUT_CREATE_MENU);
	inputCreateMenuVisualInterface->second->addEntity
	(
		std::make_shared<CreateInputForm>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
			0.5f * WindowManager::get().getWindowWidth(),
			0.15f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Create Game",
			"buttonTexture",
			Game::Status::IN_MULTIPLAYER_COLOR_MENU,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::CREATE_GAME,

			TextEntity
			(
				WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
				0.25f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"Username:"
			),

			DataBox
			(
				WindowManager::get().getWindowWidth() / 4.0f + 3.0f * WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
				0.75f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"username",
				"dataBoxTexture",
				"username"
			),

			"errorSound",
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		)
	);
	inputCreateMenuVisualInterface->second->addEntity
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

	// IM_INPUT_JOIN_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_INPUT_JOIN_MENU,

			std::make_shared<VisualInterface>(
				TexturableEntity
				(
					WindowManager::get().getWindowWidth() / 2.0f,
					WindowManager::get().getWindowHeight() / 2.0f,
					1.0f * WindowManager::get().getWindowWidth(),
					1.0f * WindowManager::get().getWindowHeight(),
					0.0f,
					"inputJoinMenuBackgroundTexture"
				)
				, false
			)
		}
	);
	const auto& inputJoinMenuVisualInterface = this->visualInterfaces.find(Game::Status::IN_INPUT_JOIN_MENU);
	inputJoinMenuVisualInterface->second->addEntity
	(
		std::make_shared<JoinInputForm>
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
			Game::Status::IN_JOINED_MULTIPLAYER_GAME,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::NONE,
			Game::MultiplayerStatus::JOIN_GAME,

			TextEntity
			(
				WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.5f * WindowManager::get().getWindowHeight(),
				0.25f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"Username:"
			),

			TextEntity
			(
				WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
				0.25f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"Server Address:"
			),

			DataBox
			(
				WindowManager::get().getWindowWidth() / 4.0f + 3.0f * WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.5f * WindowManager::get().getWindowHeight(),
				0.75f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"username",
				"dataBoxTexture",
				"username"
			),

			DataBox
			(
				WindowManager::get().getWindowWidth() / 4.0f + 3.0f * WindowManager::get().getWindowWidth() / 8.0f,
				WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
				0.75f * WindowManager::get().getWindowWidth(),
				0.075f * WindowManager::get().getWindowHeight(),
				0.0f,
				glm::vec3(1.0f, 1.0f, 1.0f),
				"arialFont",
				"serveraddress",
				"dataBoxTexture",
				"serveraddress"
			),

			"errorSound",
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		)
	);
	inputJoinMenuVisualInterface->second->addEntity
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

	// IN_MULTIPLAYER_COLOR_MENU
	this->visualInterfaces.insert(
		{
			Game::Status::IN_MULTIPLAYER_COLOR_MENU,

			std::make_shared<MultiplayerColorMenuVisualInterface>(
				MultiplayerColorMenuVisualInterface::get()
			)
		}
	);

	// IN_SINGLEPLAYER_GAME
	this->visualInterfaces.insert(
		{
			Game::Status::IN_SINGLEPLAYER_GAME,

			std::make_shared<SingleplayerGameVisualInterface>(
				SingleplayerGameVisualInterface::get()
			)
		}
	);

	// IN_CREATED_MULTIPLAYER_GAME
	this->visualInterfaces.insert(
		{
			Game::Status::IN_CREATED_MULTIPLAYER_GAME,

			std::make_shared<CreatedMultiplayerGameVisualInterface>(
				CreatedMultiplayerGameVisualInterface::get()
			)
		}
	);

	// IN_JOINED_MULTIPLAYER_GAME
	this->visualInterfaces.insert(
		{
			Game::Status::IN_JOINED_MULTIPLAYER_GAME,

			std::make_shared<JoinedMultiplayerGameVisualInterface>(
				JoinedMultiplayerGameVisualInterface::get()
			)
		}
	);
}

Game::~Game()
{

}

Game& Game::get()
{
	static Game instance;
	return instance;
}

void Game::initializeENet()
{
	if (enet_initialize() != 0)
	{
		std::cout << "Error: An error occurred while initializing ENet" << std::endl;
	}
	atexit(enet_deinitialize);
}

void Game::loadResources()
{
	WindowManager::get(); // Asigura setup-ul la OpenGL, trebuie sa fie primul apelat.
	AssetManager::get().loadResources();
	this->initializeENet(); // Enet
}

void Game::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(WindowManager::get().getWindow()))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		this->update();
		this->draw();

		glfwSwapBuffers(WindowManager::get().getWindow());
		glfwPollEvents();
	}
}

void Game::draw()
{
	const auto& visualInterface = this->visualInterfaces.find(this->status);
	if (visualInterface != this->visualInterfaces.end())
	{
		visualInterface->second->draw();
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
		visualInterface->second->update();
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

	// this->printGameStatuses(); // Doar pentru debug
}

void Game::start()
{
	this->loadResources();
	this->run();
}

void Game::setStatus(const Game::Status& status)
{
	this->status = status;

	const auto& visualInterface = this->visualInterfaces.find(this->status);
	if (visualInterface != this->visualInterfaces.end())
	{
		visualInterface->second->initialize();
	}
	else
	{
		std::cout << "Error: Game Status " << (int)visualInterface->first << " requested for changing current game status not found in Visual Interface Map" << std::endl;
	}
}

void Game::printGameStatuses()
{
	std::cout << "Game Status: ";
	switch (this->status)
	{
	case Game::Status::IN_MAIN_MENU:
		std::cout << "IN_MAIN_MENU";
		break;
	case Game::Status::IN_SETTINGS_MENU:
		std::cout << "IN_SETTINGS_MENU";
		break;
	case Game::Status::IN_MODE_MENU:
		std::cout << "IN_MODE_MENU";
		break;
	case Game::Status::IN_MULTIPLAYER_MENU:
		std::cout << "IN_MULTIPLAYER_MENU";
		break;
	case Game::Status::IN_SINGLEPLAYER_COLOR_MENU:
		std::cout << "IN_SINGLEPLAYER_COLOR_MENU";
		break;
	case Game::Status::IN_INPUT_CREATE_MENU:
		std::cout << "IN_INPUT_CREATE_MENU";
		break;
	case Game::Status::IN_INPUT_JOIN_MENU:
		std::cout << "IN_INPUT_JOIN_MENU";
		break;
	case Game::Status::IN_MULTIPLAYER_COLOR_MENU:
		std::cout << "IN_MULTIPLAYER_COLOR_MENU";
		break;
	case Game::Status::IN_SINGLEPLAYER_GAME:
		std::cout << "IN_SINGLEPLAYER_GAME";
		break;
	case Game::Status::IN_CREATED_MULTIPLAYER_GAME:
		std::cout << "IN_CREATED_MULTIPLAYER_GAME";
		break;
	case Game::Status::IN_JOINED_MULTIPLAYER_GAME:
		std::cout << "IN_JOINED_MULTIPLAYER_GAME";
		break;
	case Game::Status::EXITING:
		std::cout << "EXITING";
		break;
	default:
		std::cout << "UNKNOWN";
		break;
	}
	std::cout << std::endl;

	std::cout << "Game Mode: ";
	switch (this->mode)
	{
	case Game::Mode::SINGLEPLAYER:
		std::cout << "SINGLEPLAYER";
		break;
	case Game::Mode::MULTIPLAYER:
		std::cout << "MULTIPLAYER";
		break;
	case Game::Mode::NONE:
		std::cout << "NONE";
		break;
	default:
		std::cout << "UNKNOWN";
		break;
	}
	std::cout << std::endl;

	std::cout << "Game Color: ";
	switch (this->color)
	{
	case Game::Color::WHITE:
		std::cout << "WHITE";
		break;
	case Game::Color::BLACK:
		std::cout << "BLACK";
		break;
	case Game::Color::NONE:
		std::cout << "NONE";
		break;
	default:
		std::cout << "UNKNOWN";
		break;
	}
	std::cout << std::endl;

	std::cout << "Multiplayer Status: ";
	switch (this->multiplayerStatus)
	{
	case Game::MultiplayerStatus::NONE:
		std::cout << "NONE";
		break;
	case Game::MultiplayerStatus::CREATE_GAME:
		std::cout << "CREATE_GAME";
		break;
	case Game::MultiplayerStatus::JOIN_GAME:
		std::cout << "JOIN_GAME";
		break;
	default:
		std::cout << "UNKNOWN";
		break;
	}
	std::cout << std::endl;
}

