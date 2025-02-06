#include "CancelPawnPromotionButton.h"

#include "../../../AssetManager/AssetManager.h"

#include "../../../Renderer/Renderer.h"

#include "../../../InputManager/InputManager.h"

CancelPawnPromotionButton::CancelPawnPromotionButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
	, const std::string& soundNameWhenPressed, const std::string& textureNameWhenHovered
	, const glm::vec3& colorWhenHovered, const std::string& soundNameWhenHovered)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName)
	, status(CancelPawnPromotionButton::Status::RELEASED)
	, soundNameWhenPressed(soundNameWhenPressed)
	, textureNameWhenHovered(textureNameWhenHovered), colorWhenHovered(colorWhenHovered)
	, soundNameWhenHovered(soundNameWhenHovered)
	, recentlyInteractedWith(false)
{

}

CancelPawnPromotionButton::~CancelPawnPromotionButton()
{

}

void CancelPawnPromotionButton::draw()
{
	std::string currentTexture = "";
	if (this->status == CancelPawnPromotionButton::Status::RELEASED)
	{
		currentTexture = this->textureName;
	}
	else // if (this->status == CancelPawnPromotionButton::Status::HOVERED)
	{
		currentTexture = this->textureNameWhenHovered;
	}

	Renderer::get().draw( // background
		this->posCenterX,
		this->posCenterY,
		this->width,
		this->height,
		this->rotateAngle,
		currentTexture
	);

	glm::vec3 currentColor = glm::vec3(1.0f, 1.0f, 1.0f);
	if (this->status == CancelPawnPromotionButton::Status::RELEASED)
	{
		currentColor = this->color;
	}
	else // if (this->status == CancelPawnPromotionButton::Status::HOVERED)
	{
		currentColor = this->colorWhenHovered;
	}
	Renderer::get().drawText(
		this->posCenterX,
		this->posCenterY,
		TextEntity::TEXT_PADDING_1 * this->width,
		TextEntity::TEXT_PADDING_1 * this->height,
		this->rotateAngle,
		this->fontName,
		this->text,
		currentColor
	);
}

void CancelPawnPromotionButton::update()
{
	this->recentlyInteractedWith = false;

	if (this->isInMouseCollision())
	{
		if (InputManager::get().isLeftMouseButtonReleased()
			&& this->isInMouseLastPressedCollision())
		{
			AssetManager::get().playSound(this->soundNameWhenPressed, false, false);
			this->recentlyInteractedWith = true;
		}
		else
		{
			if (this->status == CancelPawnPromotionButton::Status::RELEASED)
			{
				AssetManager::get().playSound(this->soundNameWhenHovered, false, false);
				this->text.push_back('<');
			}
			this->status = CancelPawnPromotionButton::Status::HOVERED;
		}
	}
	else
	{
		if (this->status == CancelPawnPromotionButton::Status::HOVERED)
		{
			this->text.pop_back();
		}
		this->status = CancelPawnPromotionButton::Status::RELEASED;
	}
}