#include "Button.h"

#include "../../AssetManager/AssetManager.h"

Button::Button(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text, const Button::Status& status, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor, fontName, text)
	, status(status), gameStatusWhenPressed(gameStatusWhenPressed), soundNameWhenPressed(soundNameWhenPressed)
{

}

Button::~Button()
{

}

void Button::draw()
{
	TextEntity::draw();
}

void Button::update()
{

}