#include "JoinedMultiplayerGameVisualInterface.h"
#include "../VisualInterface.h"

#include "../../Game/Game.h"

#include "../../Client/Client.h"
#include "../../Server/Server.h"

#include "../../Entity/Button/Button.h"

#include "../../WindowManager/WindowManager.h"

#include <string>

JoinedMultiplayerGameVisualInterface::JoinedMultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
	, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
	, TextEntity serverPortTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, SingleplayerGameVisualInterface(backgroundEntity, respondsToEscapeKey, turnTextEntity, playerNameTextEntity, opponentNameTextEntity, finalMessageTextEntity)
	, serverConnectionStatusTextEntity(serverConnectionStatusTextEntity)
	, opponentConnectionStatusTextEntity(opponentConnectionStatusTextEntity)
	, serverPortTextEntity(serverPortTextEntity)
{
	this->entities.push_back
	(
		std::make_shared<Button>
		(
			WindowManager::get().getWindowWidth() / 20.0f,
			WindowManager::get().getWindowHeight() / 20.0f,
			0.075f * WindowManager::get().getWindowWidth(),
			0.075f * WindowManager::get().getWindowHeight(),
			0.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			"arialFont",
			"Back",
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
}

JoinedMultiplayerGameVisualInterface::~JoinedMultiplayerGameVisualInterface()
{

}

JoinedMultiplayerGameVisualInterface& JoinedMultiplayerGameVisualInterface::get()
{
	static JoinedMultiplayerGameVisualInterface instance(
						TexturableEntity
						(
							WindowManager::get().getWindowWidth() / 2.0f,
							WindowManager::get().getWindowHeight() / 2.0f,
							1.0f * WindowManager::get().getWindowWidth(),
							1.0f * WindowManager::get().getWindowHeight(),
							0.0f,
							"joinedMultiplayerGameBackgroundTexture"
						),
						false,


						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Turn: WHITE"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Player: Player"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							9.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Opponent: Opponent"
						),

						TextEntity
						(
							WindowManager::get().getWindowWidth() / 2.0f,
							WindowManager::get().getWindowHeight() / 2.0f,
							0.5f * WindowManager::get().getWindowWidth(),
							0.5f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(0.0f, 1.0f, 0.0f),
							"arialFont",
							"You WON!"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							7.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ServConnect: ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							8.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"OppConnect: ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							2.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ServerPort: ERROR"
						)
	);

	return instance;
}


void JoinedMultiplayerGameVisualInterface::initialize()
{
	SingleplayerGameVisualInterface::initialize();

	// SingleplayerGameVisualInterface se ocupa de update-ul si draw-ul pentru singleton-ul BoardVisualizer
	// Tot SingleplayerGameVisualInterface ruleaza si sunetul de Board Start
}

void JoinedMultiplayerGameVisualInterface::draw()
{
	SingleplayerGameVisualInterface::draw();

	this->serverConnectionStatusTextEntity.draw();
	this->opponentConnectionStatusTextEntity.draw();
	this->serverPortTextEntity.draw();
}

void JoinedMultiplayerGameVisualInterface::update()
{
	SingleplayerGameVisualInterface::update();

	this->serverConnectionStatusTextEntity.update();
	this->opponentConnectionStatusTextEntity.update();
	this->serverPortTextEntity.update();

	this->playerNameTextEntity.setText("Player: " + Game::get().getPlayerNameForMultiplayer());
}

void JoinedMultiplayerGameVisualInterface::setServerStatus(bool statusOk)
{
	if (statusOk)
	{
		this->serverConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->serverConnectionStatusTextEntity.setText("Server connection: OK");
	}
	else
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("Opponent Connection: ERROR");

		this->serverConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->serverConnectionStatusTextEntity.setText("Server Connection: ERROR");
	}
}

void JoinedMultiplayerGameVisualInterface::setOpponentStatus(bool statusOk)
{
	if (statusOk)
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("Opponent Connection: OK");
	}
	else
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("Opponent Connection: ERROR");
	}
}