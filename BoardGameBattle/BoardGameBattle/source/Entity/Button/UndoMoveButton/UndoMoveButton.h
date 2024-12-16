#pragma once

#include "../Button.h"

class UndoMoveButton : virtual public Button
{
private:

public:
	UndoMoveButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
		, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
		, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
		, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
		, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed);
	virtual ~UndoMoveButton();

	virtual void draw() override;
	virtual void update() override;
};