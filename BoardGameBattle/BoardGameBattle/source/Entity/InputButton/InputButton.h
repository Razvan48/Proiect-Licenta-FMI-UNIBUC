#pragma once

#include "../Button/Button.h"

class InputButton : virtual public Button
{
protected:
	std::string input;
	TextEntity textEntity;

public:
	InputButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text, const Button::Status& status, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed, const std::string& input, TextEntity textEntity);
	virtual ~InputButton();

	virtual void draw() override;
	virtual void update() override;

	inline const std::string& getInput() const { return this->input; }
	inline TextEntity& getTextEntity() { return this->textEntity; }
};