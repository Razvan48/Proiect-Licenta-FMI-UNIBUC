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
	, playerName("")
	, serverAddress("")
	, color("")
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
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"Turn: ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"Player: ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							9.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.1f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"Opp: ERROR"
						),

						TextEntity
						(
							WindowManager::get().getWindowWidth() / 2.0f,
							WindowManager::get().getWindowHeight() / 2.0f,
							0.5f * WindowManager::get().getWindowWidth(),
							0.5f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR!"
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
							"ServConn: ERROR"
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
							"OppConn: ERROR"
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
							"ServPort: ERROR"
						)
	);

	return instance;
}


void JoinedMultiplayerGameVisualInterface::initialize()
{
	SingleplayerGameVisualInterface::initialize();

	// SingleplayerGameVisualInterface se ocupa de update-ul si draw-ul pentru singleton-ul BoardVisualizer
	// + draw-ul pentru finalMessage tot in SingleplayerGameVisualInterface are loc
	// Tot SingleplayerGameVisualInterface ruleaza si sunetul de Board Start

	this->turnTextEntity.setText("Turn: ERROR");
	this->turnTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->playerNameTextEntity.setText("Player: ERROR");
	this->playerNameTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->opponentNameTextEntity.setText("Opp: ERROR");
	this->opponentNameTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->finalMessageTextEntity.setText("ERROR!");
	this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->serverConnectionStatusTextEntity.setText("ServConn: ERROR");
	this->serverConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->opponentConnectionStatusTextEntity.setText("OppConn: ERROR");
	this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->serverPortTextEntity.setText("ServPort: ERROR");
	this->serverPortTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	///

	this->playerNameTextEntity.setText("Player: " + this->playerName);
	this->playerNameTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));



	int lastPositionTwoPoints = -1;
	for (int i = 0; i < this->serverAddress.size(); ++i)
	{
		if (this->serverAddress[i] == ':')
			lastPositionTwoPoints = i;
	}

	std::string serverIP = this->serverAddress.substr(0, lastPositionTwoPoints);
	int serverPort = 0;
	for (int i = lastPositionTwoPoints + 1; i < this->serverAddress.size(); ++i)
	{
		serverPort = serverPort * 10 + (int)(this->serverAddress[i] - '0');
	}

	this->serverPortTextEntity.setText("ServPort: " + std::to_string(serverPort));
	this->serverPortTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));

	Client::get().start(serverIP, serverPort, this->playerName, this->color);
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

	// this->playerNameTextEntity.setText("Player: " + this->playerName);

	Client::get().update();
}

void JoinedMultiplayerGameVisualInterface::setServerStatus(bool statusOk)
{
	if (statusOk)
	{
		this->serverConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->serverConnectionStatusTextEntity.setText("ServConn: OK");
	}
	else
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("OppConn: ERROR");

		this->serverConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->serverConnectionStatusTextEntity.setText("ServConn: ERROR");
	}
}

void JoinedMultiplayerGameVisualInterface::setOpponentStatus(bool statusOk)
{
	if (statusOk)
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("OppConn: OK");
	}
	else
	{
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->opponentConnectionStatusTextEntity.setText("OppConn: ERROR");
	}
}