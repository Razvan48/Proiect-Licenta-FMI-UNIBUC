#include "MultiplayerColorMenuVisualInterface.h"

#include "../../WindowManager/WindowManager.h"

#include "../../Entity/TexturableEntity/TexturableEntity.h"

#include "../CreatedMultiplayerGameVisualInterface/CreatedMultiplayerGameVisualInterface.h"

#include "../../InputManager/InputManager.h"

#include <iostream>

MultiplayerColorMenuVisualInterface::MultiplayerColorMenuVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, whiteButton(
		WindowManager::get().getWindowWidth() / 2.0f,
		WindowManager::get().getWindowHeight() / 8.0f + 0.4f * WindowManager::get().getWindowHeight(),
		0.5f * WindowManager::get().getWindowWidth(),
		0.15f * WindowManager::get().getWindowHeight(),
		0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		"defaultFont",
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
	),
	blackButton(
		WindowManager::get().getWindowWidth() / 2.0f,
		WindowManager::get().getWindowHeight() / 8.0f + 0.2f * WindowManager::get().getWindowHeight(),
		0.5f * WindowManager::get().getWindowWidth(),
		0.15f * WindowManager::get().getWindowHeight(),
		0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		"defaultFont",
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
{
	this->entities.push_back // Butonul de Back
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 2.0f,
			WindowManager::get().getWindowHeight() / 8.0f,
			0.25f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"defaultFont",
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

std::shared_ptr<MultiplayerColorMenuVisualInterface> MultiplayerColorMenuVisualInterface::get()
{
	static std::shared_ptr<MultiplayerColorMenuVisualInterface> instance = std::make_shared<MultiplayerColorMenuVisualInterface>
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

	this->whiteButton.draw();
	this->blackButton.draw();
}

void MultiplayerColorMenuVisualInterface::update()
{
	VisualInterface::update();

	// Liniile astea trebuie sa fie primele, ca altfel butoanele cand dau update vor schimba statusul,
	// ceea ce va duce la apelul de initialize() fara sa apucam sa populam string-urile.
	if (this->whiteButton.anInteractionWillHappen())
		CreatedMultiplayerGameVisualInterface::get()->setColor("white");
	else if (this->blackButton.anInteractionWillHappen())
		CreatedMultiplayerGameVisualInterface::get()->setColor("black");

	this->whiteButton.update();
	this->blackButton.update();
}