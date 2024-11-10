#include "MultiplayerColorMenuVisualInterface.h"

#include "../../WindowManager/WindowManager.h"

#include "../../Entity/TexturableEntity/TexturableEntity.h"
#include "../../Entity/Button/Button.h"

MultiplayerColorMenuVisualInterface::MultiplayerColorMenuVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
{
	this->entities.push_back
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
			Game::Status::IN_CREATED_MULTIPLAYER_GAME,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::WHITE,
			Game::MultiplayerStatus::CREATE_GAME
		)
	);

	this->entities.push_back
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
			Game::Status::IN_CREATED_MULTIPLAYER_GAME,
			"buttonPressedSound",
			"buttonHoveredTexture",
			glm::vec3(0.75f, 0.75f, 0.75f),
			"buttonHoveredSound",
			Game::Mode::MULTIPLAYER,
			Game::Color::BLACK,
			Game::MultiplayerStatus::CREATE_GAME
		)
	);

	this->entities.push_back
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
}

MultiplayerColorMenuVisualInterface::~MultiplayerColorMenuVisualInterface()
{

}

MultiplayerColorMenuVisualInterface& MultiplayerColorMenuVisualInterface::get()
{
	static MultiplayerColorMenuVisualInterface instance
	(
		TexturableEntity
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 2.0f,
			1.0f * WindowManager::get().getWindowWidth(),
			1.0f * WindowManager::get().getWindowHeight(),
			0.0f,
			"multiplayerColorMenuBackgroundTexture"
		)
		, false
	);

	return instance;

}

void MultiplayerColorMenuVisualInterface::initialize()
{
	VisualInterface::initialize();
}

void MultiplayerColorMenuVisualInterface::draw()
{
	VisualInterface::draw();
}

void MultiplayerColorMenuVisualInterface::update()
{
	VisualInterface::update();
}