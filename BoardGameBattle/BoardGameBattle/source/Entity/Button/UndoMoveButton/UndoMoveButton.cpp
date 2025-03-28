#include "UndoMoveButton.h"

#include "../../../AssetManager/AssetManager.h"
#include "../../../InputManager/InputManager.h"

#include "../../../BoardVisualizer/BoardVisualizer.h"
#include "../../../BoardManager/BoardManager.h"

#include "../../../GameAgent/GameAgentSelector/GameAgentSelector.h"

UndoMoveButton::UndoMoveButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
	, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
	, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
	, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
	, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName)
	, Button(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName
		, gameStatusWhenPressed, soundNameWhenPressed, textureNameWhenHovered, colorWhenHovered, soundNameWhenHovered
		, gameModeWhenPressed, gameColorWhenPressed, gameMultiplayerStatusWhenPressed)
{

}


UndoMoveButton::~UndoMoveButton()
{

}

void UndoMoveButton::draw()
{
	Button::draw();
}

void UndoMoveButton::update()
{
	this->recentlyInteractedWith = false;

	if (this->isInMouseCollision())
	{
		if (InputManager::get().isLeftMouseButtonReleased()
			&& this->isInMouseLastPressedCollision())
		{
			AssetManager::get().playSound(this->soundNameWhenPressed, false, false);
			this->recentlyInteractedWith = true;

			if (	!
					(Game::get().getColor() == Game::Color::BLACK && BoardVisualizer::get().getLastMoveFromHistorySize() == 1)
				)
			{
				// Marcam Agentul ca fiind inactiv + Resetare
				GameAgentSelector::get().setIsFindingBestMove(false);
				GameAgentSelector::get().setBestMove(std::vector<std::pair<char, int>>());
				GameAgentSelector::get().setIsFindBestMoveCancelled(true);

				GameAgentSelector::get().setIsEstimating(false);
				GameAgentSelector::get().resetEstimation();
				GameAgentSelector::get().setIsEstimateCancelled(true);

				BoardVisualizer::get().popLastMoveFromHistory();
				BoardManager::get().popLastConfigurationMetadataFromHistory();

				if (
					(BoardManager::get().getConfigurationMetadata().whiteTurn && Game::get().getColor() == Game::Color::BLACK)
					||
					(!BoardManager::get().getConfigurationMetadata().whiteTurn && Game::get().getColor() == Game::Color::WHITE)
					)
				{
					BoardVisualizer::get().popLastMoveFromHistory();
					BoardManager::get().popLastConfigurationMetadataFromHistory();
				}
			}
		}
		else
		{
			if (this->status == Button::Status::RELEASED)
			{
				AssetManager::get().playSound(this->soundNameWhenHovered, false, false);
				this->text.push_back('<');
			}
			this->status = Button::Status::HOVERED;
		}
	}
	else
	{
		if (this->status == Button::Status::HOVERED)
		{
			this->text.pop_back();
		}
		this->status = Button::Status::RELEASED;
	}
}