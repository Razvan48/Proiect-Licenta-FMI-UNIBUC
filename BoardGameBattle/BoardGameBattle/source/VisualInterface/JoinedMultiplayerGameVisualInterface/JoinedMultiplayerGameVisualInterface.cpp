#include "JoinedMultiplayerGameVisualInterface.h"
#include "../VisualInterface.h"

#include "../../Game/Game.h"

#include "../../Client/Client.h"
#include "../../Server/Server.h"

#include "../../Entity/Button/Button.h"

#include "../../WindowManager/WindowManager.h"

#include "../../BoardManager/BoardManager.h"

#include "../../BoardVisualizer/BoardVisualizer.h"

#include "../../GameMetadata/GameMetadata.h"

#include <string>

JoinedMultiplayerGameVisualInterface::JoinedMultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey
	, TextEntity turnLabelTextEntity, TextEntity playerNameLabelTextEntity, TextEntity opponentNameLabelTextEntity
	, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
	, TextEntity serverConnectionStatusLabelTextEntity, TextEntity opponentConnectionStatusLabelTextEntity, TextEntity serverPortLabelTextEntity
	, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
	, TextEntity serverPortTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, SingleplayerGameVisualInterface(backgroundEntity, respondsToEscapeKey, turnLabelTextEntity, playerNameLabelTextEntity, opponentNameLabelTextEntity, turnTextEntity, playerNameTextEntity, opponentNameTextEntity, finalMessageTextEntity)
	, serverConnectionStatusLabelTextEntity(serverConnectionStatusLabelTextEntity)
	, opponentConnectionStatusLabelTextEntity(opponentConnectionStatusLabelTextEntity)
	, serverPortLabelTextEntity(serverPortLabelTextEntity)
	, serverConnectionStatusTextEntity(serverConnectionStatusTextEntity)
	, opponentConnectionStatusTextEntity(opponentConnectionStatusTextEntity)
	, serverPortTextEntity(serverPortTextEntity)
	, playerName("")
	, serverAddress("")
	, hasToSendBoardConfiguration(false)
	, clientColorSet(false)
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

std::shared_ptr<JoinedMultiplayerGameVisualInterface> JoinedMultiplayerGameVisualInterface::get()
{
	static std::shared_ptr<JoinedMultiplayerGameVisualInterface> instance = std::make_shared<JoinedMultiplayerGameVisualInterface>
	(
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
							WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Turn:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Player:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							9.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Opponent:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							9.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
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
							7.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Server Connection:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							8.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Opponent Connection:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							2.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 1.0f, 1.0f),
							"arialFont",
							"Server Port:"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							7.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							8.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
						),

						TextEntity
						(
							9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
							2.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
							0.1f * WindowManager::get().getWindowWidth(),
							0.05f * WindowManager::get().getWindowHeight(),
							0.0f,
							glm::vec3(1.0f, 0.0f, 0.0f),
							"arialFont",
							"ERROR"
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


	this->turnTextEntity.setText("ERROR");
	this->turnTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->playerNameTextEntity.setText("ERROR");
	this->playerNameTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->opponentNameTextEntity.setText("ERROR");
	this->opponentNameTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->finalMessageTextEntity.setText("ERROR!");
	this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->serverConnectionStatusTextEntity.setText("ERROR");
	this->serverConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->opponentConnectionStatusTextEntity.setText("ERROR");
	this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	this->serverPortTextEntity.setText("ERROR");
	this->serverPortTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));



	this->hasToSendBoardConfiguration = false;
	this->clientColorSet = false;

	///

	this->playerNameTextEntity.setText(this->playerName);
	this->playerNameTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));


	// Server Address (IP + PORT)
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

	this->serverPortTextEntity.setText(std::to_string(serverPort));
	this->serverPortTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));

	Client::get().start(serverIP, serverPort, this->playerName, "", "");
}

void JoinedMultiplayerGameVisualInterface::draw()
{
	SingleplayerGameVisualInterface::draw();

	this->serverConnectionStatusLabelTextEntity.draw();
	this->opponentConnectionStatusLabelTextEntity.draw();
	this->serverPortLabelTextEntity.draw();

	this->serverConnectionStatusTextEntity.draw();
	this->opponentConnectionStatusTextEntity.draw();
	this->serverPortTextEntity.draw();
}

void JoinedMultiplayerGameVisualInterface::update()
{
	SingleplayerGameVisualInterface::update();

	this->serverConnectionStatusLabelTextEntity.update();
	this->opponentConnectionStatusLabelTextEntity.update();
	this->serverPortLabelTextEntity.update();

	this->serverConnectionStatusTextEntity.update();
	this->opponentConnectionStatusTextEntity.update();
	this->serverPortTextEntity.update();



	Client::get().update();

	if (this->hasToSendBoardConfiguration)
	{
		float timeWhenMessageSent = 0.0f; // Nu ne intereseaza timpul
		Client::get().setLastKnownBoardConfiguration(BoardManager::get().getPiecesConfiguration());
		Client::get().sendMessage("boardConfiguration:" + BoardManager::get().getPiecesConfiguration(), this->hasToSendBoardConfiguration, timeWhenMessageSent);
	}

	if (BoardManager::get().getPiecesConfiguration() != Client::get().getLastKnownBoardConfiguration())
		BoardManager::get().setPiecesConfiguration(Client::get().getLastKnownBoardConfiguration());


	if (!this->clientColorSet && Client::get().getColor() != "")
	{
		BoardVisualizer::get().initialize();
		this->clientColorSet = true;
	}

	// Turn
	std::string clientBoardConfiguration = Client::get().getLastKnownBoardConfiguration();
	if (clientBoardConfiguration != "")
	{
		if (clientBoardConfiguration[GameMetadata::NUM_TILES_HEIGHT * GameMetadata::NUM_TILES_WIDTH + GameMetadata::NUM_CASTLING_MOVES] == 'w')
		{
			this->turnTextEntity.setText("WHITE");
			this->turnTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else
		{
			this->turnTextEntity.setText("BLACK");
			this->turnTextEntity.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		this->turnTextEntity.setText("ERROR");
		this->turnTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Opponent
	if (Client::get().getOpponentName() != "")
	{
		this->opponentNameTextEntity.setText(Client::get().getOpponentName());
		this->opponentNameTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		this->opponentNameTextEntity.setText("ERROR");
		this->opponentNameTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Server Connection
	if (Client::get().getWorkingServerConnection())
	{
		this->serverConnectionStatusTextEntity.setText("OK");
		this->serverConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		this->serverConnectionStatusTextEntity.setText("ERROR");
		this->serverConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Opponent Connection
	if (Client::get().getWorkingOpponentConnection())
	{
		this->opponentConnectionStatusTextEntity.setText("OK");
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		this->opponentConnectionStatusTextEntity.setText("ERROR");
		this->opponentConnectionStatusTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

