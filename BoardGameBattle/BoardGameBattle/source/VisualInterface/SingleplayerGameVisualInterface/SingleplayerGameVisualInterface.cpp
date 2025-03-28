#include <sstream>
#include <iomanip>

#include "SingleplayerGameVisualInterface.h"

#include "../../BoardVisualizer/BoardVisualizer.h"
#include "../../BoardManager/BoardManager.h"
#include "../../Game/Game.h"
#include "../../WindowManager/WindowManager.h"

#include "../../AssetManager/AssetManager.h"

#include "../../Entity/Button/Button.h"

#include "../../RandomGenerator/RandomGenerator.h"

SingleplayerGameVisualInterface::SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey
	, TextEntity turnLabelTextEntity, TextEntity playerNameLabelTextEntity, TextEntity opponentNameLabelTextEntity
	, TextEntity turnTextEntity
	, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
	, turnLabelTextEntity(turnLabelTextEntity)
	, playerNameLabelTextEntity(playerNameLabelTextEntity)
	, opponentNameLabelTextEntity(opponentNameLabelTextEntity)

	, estimationLabelTextEntity
	(
		9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
		5.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.75f * WindowManager::get().getWindowHeight() / 10.0f,
		0.1f * WindowManager::get().getWindowWidth(),
		0.05f * WindowManager::get().getWindowHeight(),
		0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		"arialFont",
		"Estimation:"
	)

	, turnTextEntity(turnTextEntity)
	, playerNameTextEntity(playerNameTextEntity)
	, opponentNameTextEntity(opponentNameTextEntity)

	, estimationTextEntity
	(
		9.0f * WindowManager::get().getWindowWidth() / 10.0f + 0.5f * WindowManager::get().getWindowWidth() / 10.0f,
		5.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.25f * WindowManager::get().getWindowHeight() / 10.0f,
		0.1f * WindowManager::get().getWindowWidth(),
		0.05f * WindowManager::get().getWindowHeight(),
		0.0f,
		glm::vec3(1.0f, 0.0f, 0.0f),
		"arialFont",
		"ERROR"
	)

	, undoMoveButton
	(
		19.0f * WindowManager::get().getWindowWidth() / 20.0f,
		4.0f * WindowManager::get().getWindowHeight() / 10.0f + 0.5f * WindowManager::get().getWindowHeight() / 10.0f,
		0.075f * WindowManager::get().getWindowWidth(),
		0.075f * WindowManager::get().getWindowHeight(),
		0.0f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		"arialFont",
		"Undo",
		"buttonTexture",
		Game::Status::NONE,
		"buttonPressedSound",
		"buttonHoveredTexture",
		glm::vec3(0.75f, 0.75f, 0.75f),
		"buttonHoveredSound",
		Game::Mode::SINGLEPLAYER,
		Game::Color::NONE,
		Game::MultiplayerStatus::NONE
	)

	, finalMessageTextEntity(finalMessageTextEntity)
	, displayFinalMessage(false)
	, boardStartSoundName("boardStartSound")

	, finalMessage(FinalMessage::NOT_FINISHED)
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

	this->estimationTextEntity.setText("0");
	this->setEstimationObsolete(true);

	this->setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED);

	AssetManager::get().playSound(this->boardStartSoundName, false, false);
}

void SingleplayerGameVisualInterface::draw()
{
	VisualInterface::draw();

	this->turnLabelTextEntity.draw();
	this->playerNameLabelTextEntity.draw();
	this->opponentNameLabelTextEntity.draw();
	this->estimationLabelTextEntity.draw();

	this->turnTextEntity.draw();
	this->playerNameTextEntity.draw();
	this->opponentNameTextEntity.draw();
	this->estimationTextEntity.draw();

	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER)
		this->undoMoveButton.draw();

	BoardVisualizer::get().draw();

	if (this->displayFinalMessage)
		this->finalMessageTextEntity.draw();
}

void SingleplayerGameVisualInterface::update()
{
	VisualInterface::update();

	this->turnLabelTextEntity.update();
	this->playerNameLabelTextEntity.update();
	this->opponentNameLabelTextEntity.update();
	this->estimationLabelTextEntity.update();

	this->turnTextEntity.update();
	this->playerNameTextEntity.update();
	this->opponentNameTextEntity.update();
	this->estimationTextEntity.update();

	if (Game::get().getMode() == Game::Mode::SINGLEPLAYER && !BoardVisualizer::get().getPawnPromotionMenuActive())
		this->undoMoveButton.update();

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

void SingleplayerGameVisualInterface::setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage finalMessage)
{
	if (finalMessage == SingleplayerGameVisualInterface::FinalMessage::WON)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		this->finalMessageTextEntity.setText("You WON!");

		AssetManager::get().playSound("win" + std::to_string(RandomGenerator::randomUniformInt(0, 1)) + "Sound", false, false);

		this->displayFinalMessage = true;
	}
	else if (finalMessage == SingleplayerGameVisualInterface::FinalMessage::LOST)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->finalMessageTextEntity.setText("You LOST!");

		AssetManager::get().playSound("lose" + std::to_string(RandomGenerator::randomUniformInt(0, 0)) + "Sound", false, false);

		this->displayFinalMessage = true;
	}
	else if (finalMessage == SingleplayerGameVisualInterface::FinalMessage::DRAW)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 1.0f, 0.0f)); // galben
		this->finalMessageTextEntity.setText("DRAW!");

		AssetManager::get().playSound("draw" + std::to_string(RandomGenerator::randomUniformInt(0, 3)) + "Sound", false, false);

		this->displayFinalMessage = true;
	}
	else if (finalMessage == SingleplayerGameVisualInterface::FinalMessage::IN_PAWN_PROMOTION_MENU)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		this->finalMessageTextEntity.setText("Please Select Promotion Piece");

		this->displayFinalMessage = true;
	}
	else // if (finalMessage == SingleplayerGameVisualInterface::FinalMessage::NOT_FINISHED)
	{
		this->finalMessageTextEntity.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		this->finalMessageTextEntity.setText("ERROR!");

		this->displayFinalMessage = false;
	}

	this->finalMessage = finalMessage;
}

void SingleplayerGameVisualInterface::setEstimationObsolete(bool isObsolete)
{
	if (isObsolete)
	{
		this->estimationLabelTextEntity.setColor(glm::vec3(0.5f, 0.5f, 0.5f));
		this->estimationTextEntity.setColor(glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		// INFO: std::stof converteste un string la float.
		this->setEstimationValue(std::stof(this->estimationTextEntity.getText()));
	}
}

void SingleplayerGameVisualInterface::setEstimationValue(float estimation)
{
	this->estimationLabelTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	if (estimation > 0.0f)
		this->estimationTextEntity.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	else if (estimation < 0.0f)
		this->estimationTextEntity.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	else
		this->estimationTextEntity.setColor(glm::vec3(0.5f, 0.5f, 0.5f));


	const int MAX_ESTIMATION_LENGTH = 6;
	std::ostringstream outputStream;
	outputStream << std::fixed << std::setprecision(MAX_ESTIMATION_LENGTH) << estimation;
	std::string estimationString = outputStream.str();
	this->estimationTextEntity.setText(estimationString);
}


