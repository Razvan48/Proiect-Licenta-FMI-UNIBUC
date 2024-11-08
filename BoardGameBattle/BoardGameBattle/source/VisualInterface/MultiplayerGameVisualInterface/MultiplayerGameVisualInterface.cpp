#include "MultiplayerGameVisualInterface.h"
#include "../VisualInterface.h"

#include "../../Game/Game.h"

MultiplayerGameVisualInterface::MultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
	, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
	, TextEntity serverPortTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, SingleplayerGameVisualInterface(backgroundEntity, respondsToEscapeKey, turnTextEntity, playerNameTextEntity, opponentNameTextEntity, finalMessageTextEntity)
	, serverConnectionStatusTextEntity(serverConnectionStatusTextEntity)
	, opponentConnectionStatusTextEntity(opponentConnectionStatusTextEntity)
	, serverPortTextEntity(serverPortTextEntity)
{

}

MultiplayerGameVisualInterface::~MultiplayerGameVisualInterface()
{

}

void MultiplayerGameVisualInterface::initialize()
{
	SingleplayerGameVisualInterface::initialize();

	// SingleplayerGameVisualInterface se ocupa de update-ul si draw-ul pentru singleton-ul BoardVisualizer
}

void MultiplayerGameVisualInterface::draw()
{
	SingleplayerGameVisualInterface::draw();

	this->serverConnectionStatusTextEntity.draw();
	this->opponentConnectionStatusTextEntity.draw();
	this->serverPortTextEntity.draw();
}

void MultiplayerGameVisualInterface::update()
{
	SingleplayerGameVisualInterface::update();

	this->serverConnectionStatusTextEntity.update();
	this->opponentConnectionStatusTextEntity.update();
	this->serverPortTextEntity.update();

	this->playerNameTextEntity.setText("Player: " + Game::get().getPlayerNameForMultiplayer());
}

void MultiplayerGameVisualInterface::setServerStatus(bool statusOk)
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

void MultiplayerGameVisualInterface::setOpponentStatus(bool statusOk)
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