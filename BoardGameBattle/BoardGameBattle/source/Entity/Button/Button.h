#pragma once

#include "../TextOnBackgroundEntity/TextOnBackgroundEntity.h"
#include "../../Game/Game.h"

class Button : virtual public TextOnBackgroundEntity
{
public:
	enum class Status
	{
		RELEASED,
		HOVERED,
	};

protected:
	Button::Status status;

	Game::Status gameStatusWhenPressed;
	std::string soundNameWhenPressed;

	std::string textureNameWhenHovered;
	glm::vec3 colorWhenHovered;
	std::string soundNameWhenHovered;

	bool recentlyPressed;

	Game::Mode gameModeWhenPressed;
	Game::Color gameColorWhenPressed;
	Game::MultiplayerStatus gameMultiplayerStatusWhenPressed;

public:
	Button(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
		, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
		, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
		, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
		, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed);
	virtual ~Button();

	virtual void draw() override;
	virtual void update() override;

	bool isInMouseCollision() const;

	inline bool getRecentlyPressed() const { return this->recentlyPressed; }
};