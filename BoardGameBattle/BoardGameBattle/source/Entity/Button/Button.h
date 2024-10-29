#pragma once

#include "../TextEntity/TextEntity.h"
#include "../../Game/Game.h"

class Button : virtual public TextEntity
{
public:
	enum class Status
	{
		RELEASED,
		PRESSED,
	};

protected:
	Button::Status status;

	Game::Status gameStatusWhenPressed;
	std::string soundNameWhenPressed;

public:
	Button(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text, const Button::Status& status, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed);
	virtual ~Button();

	virtual void draw() override;
	virtual void update() override;
};