#include "PawnPromotionButton.h"

#include "../../../AssetManager/AssetManager.h"

#include "../../../Renderer/Renderer.h"

#include "../../../InputManager/InputManager.h"

PawnPromotionButton::PawnPromotionButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, const std::string& backgroundTextureName, const std::string& soundNameWhenPressed, const std::string& backgroundTextureNameWhenHovered, const std::string& soundNameWhenHovered)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextureOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName, backgroundTextureName)
	, status(PawnPromotionButton::Status::RELEASED)
	, soundNameWhenPressed(soundNameWhenPressed)
	, backgroundTextureNameWhenHovered(backgroundTextureNameWhenHovered)
	, soundNameWhenHovered(soundNameWhenHovered)
	, recentlyInteractedWith(false)
{

}

PawnPromotionButton::~PawnPromotionButton()
{

}

void PawnPromotionButton::draw()
{
	std::string currentTexture = "";
	if (this->status == PawnPromotionButton::Status::RELEASED)
	{
		currentTexture = this->backgroundTextureName;
	}
	else // if (this->status == PawnPromotionButton::Status::HOVERED)
	{
		currentTexture = this->backgroundTextureNameWhenHovered;
	}

	Renderer::get().draw( // background
		this->posCenterX,
		this->posCenterY,
		this->width,
		this->height,
		this->rotateAngle,
		currentTexture
	);

	Renderer::get().draw( // texture
		this->posCenterX,
		this->posCenterY,
		TextureOnBackgroundEntity::TEXTURE_PADDING_1 * this->width,
		TextureOnBackgroundEntity::TEXTURE_PADDING_1 * this->height,
		this->rotateAngle,
		this->textureName
	);
}

void PawnPromotionButton::update()
{
	TextureOnBackgroundEntity::update();

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
			if (this->status == PawnPromotionButton::Status::RELEASED)
			{
				AssetManager::get().playSound(this->soundNameWhenHovered, false, false);
			}
			this->status = PawnPromotionButton::Status::HOVERED;
		}
	}
	else
	{
		this->status = PawnPromotionButton::Status::RELEASED;
	}
}