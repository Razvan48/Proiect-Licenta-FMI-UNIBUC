#include "InputButton.h"

InputButton::InputButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text, const Button::Status& status, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed, const std::string& input, TextEntity textEntity)
	: Entity(posCenterX, posCenterY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor, fontName, text)
	, Button(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor, fontName, text, status, gameStatusWhenPressed, soundNameWhenPressed)
	, input(input), textEntity(textEntity)
{

}

InputButton::~InputButton()
{

}

void InputButton::draw()
{
	Button::draw();

	this->textEntity.draw();
}

void InputButton::update()
{

}