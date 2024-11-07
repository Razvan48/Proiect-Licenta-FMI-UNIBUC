#include "SingleplayerGameVisualInterface.h"

#include "../../BoardVisualizer/BoardVisualizer.h"
#include "../../Game/Game.h"

SingleplayerGameVisualInterface::SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, turnTextEntity(turnTextEntity)
	, playerNameTextEntity(playerNameTextEntity)
	, opponentNameTextEntity(opponentNameTextEntity)
	, finalMessageTextEntity(finalMessageTextEntity)
{
	this->finalMessageTextEntity.setRequestedToBeHidden(true); // Sa nu apara din prima
}

SingleplayerGameVisualInterface::~SingleplayerGameVisualInterface()
{

}

void SingleplayerGameVisualInterface::draw()
{
	VisualInterface::draw();

	this->turnTextEntity.draw();
	this->playerNameTextEntity.draw();
	this->opponentNameTextEntity.draw();
	this->finalMessageTextEntity.draw();

	BoardVisualizer::get().draw();
}

void SingleplayerGameVisualInterface::update()
{
	VisualInterface::update();

	this->turnTextEntity.update();
	this->playerNameTextEntity.update();
	this->opponentNameTextEntity.update();
	this->finalMessageTextEntity.update();

	if (Game::get().getStatus() != Game::get().getPreviousStatus())
	{
		BoardVisualizer::get().initialize();
	}
}

void SingleplayerGameVisualInterface::setTurn(bool whiteTurn)
{
	if (whiteTurn)
	{
		this->turnTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		this->turnTextEntity.setText("Turn: WHITE");
	}
	else
	{
		this->turnTextEntity.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
		this->turnTextEntity.setText("Turn: BLACK");

	}
}

void SingleplayerGameVisualInterface::setPlayerName(const std::string& playerName)
{
	this->playerNameTextEntity.setText(playerName);
}

void SingleplayerGameVisualInterface::setOpponentName(const std::string& opponentName)
{
	this->opponentNameTextEntity.setText(opponentName);
}

void SingleplayerGameVisualInterface::setFinalMessage(bool hasWon)
{
	this->finalMessageTextEntity.setRequestedToBeHidden(false);

	if (hasWon)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->finalMessageTextEntity.setText("You WON!");
	}
	else
	{
		this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->finalMessageTextEntity.setText("You LOST!");
	}
}