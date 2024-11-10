#include "SingleplayerGameVisualInterface.h"

#include "../../BoardVisualizer/BoardVisualizer.h"
#include "../../BoardManager/BoardManager.h"
#include "../../Game/Game.h"
#include "../../WindowManager/WindowManager.h"

#include "../../AssetManager/AssetManager.h"

#include "../../Entity/Button/Button.h"

SingleplayerGameVisualInterface::SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, turnTextEntity(turnTextEntity)
	, playerNameTextEntity(playerNameTextEntity)
	, opponentNameTextEntity(opponentNameTextEntity)
	, finalMessageTextEntity(finalMessageTextEntity)
	, displayFinalMessage(false)
	, boardStartSoundName("boardStartSound")
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
}

SingleplayerGameVisualInterface::~SingleplayerGameVisualInterface()
{

}

SingleplayerGameVisualInterface& SingleplayerGameVisualInterface::get()
{
	static SingleplayerGameVisualInterface instance(
						TexturableEntity
						(
							WindowManager::get().getWindowWidth() / 2.0f,
							WindowManager::get().getWindowHeight() / 2.0f,
							1.0f * WindowManager::get().getWindowWidth(),
							1.0f * WindowManager::get().getWindowHeight(),
							0.0f,
							"singleplayerGameBackgroundTexture"
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
							"Opponent: BOT"
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
						)
		);

	return instance;
}

void SingleplayerGameVisualInterface::initialize()
{
	VisualInterface::initialize();

	BoardVisualizer::get().initialize();
	BoardManager::get().initialize();

	AssetManager::get().playSound(this->boardStartSoundName, false);
}

void SingleplayerGameVisualInterface::draw()
{
	VisualInterface::draw();

	this->turnTextEntity.draw();
	this->playerNameTextEntity.draw();
	this->opponentNameTextEntity.draw();

	if (this->displayFinalMessage)
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

	BoardVisualizer::get().update();
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
	this->displayFinalMessage = true;

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