#include "SingleplayerGameVisualInterface.h"

#include "../../BoardVisualizer/BoardVisualizer.h"
#include "../../BoardManager/BoardManager.h"
#include "../../Game/Game.h"
#include "../../WindowManager/WindowManager.h"

#include "../../AssetManager/AssetManager.h"

#include "../../Entity/Button/Button.h"

SingleplayerGameVisualInterface::SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey
	, TextEntity turnLabelTextEntity, TextEntity playerNameLabelTextEntity, TextEntity opponentNameLabelTextEntity
	, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, turnLabelTextEntity(turnLabelTextEntity)
	, playerNameLabelTextEntity(playerNameLabelTextEntity)
	, opponentNameLabelTextEntity(opponentNameLabelTextEntity)
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

std::shared_ptr<SingleplayerGameVisualInterface> SingleplayerGameVisualInterface::get()
{
	static std::shared_ptr<SingleplayerGameVisualInterface> instance = std::make_shared<SingleplayerGameVisualInterface>
	(
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
						)
	);

	return instance;
}

void SingleplayerGameVisualInterface::initialize()
{
	VisualInterface::initialize();

	BoardVisualizer::get().initialize();
	BoardManager::get().initialize();

	this->turnTextEntity.setText("WHITE");
	this->turnTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	this->playerNameTextEntity.setText("Player");
	this->playerNameTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	this->opponentNameTextEntity.setText("BOT");
	this->opponentNameTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	this->finalMessageTextEntity.setText("ERROR!");
	this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	AssetManager::get().playSound(this->boardStartSoundName, false, false);
}

void SingleplayerGameVisualInterface::draw()
{
	VisualInterface::draw();

	this->turnLabelTextEntity.draw();
	this->playerNameLabelTextEntity.draw();
	this->opponentNameLabelTextEntity.draw();

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

	this->turnLabelTextEntity.update();
	this->playerNameLabelTextEntity.update();
	this->opponentNameLabelTextEntity.update();

	this->turnTextEntity.update();
	this->playerNameTextEntity.update();
	this->opponentNameTextEntity.update();

	this->finalMessageTextEntity.update();

	BoardVisualizer::get().update();

	if (BoardManager::get().getWhiteTurn())
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

