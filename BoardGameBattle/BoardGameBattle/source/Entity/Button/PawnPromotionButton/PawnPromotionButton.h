#pragma once

#include <string>

#include "../../TextureOnBackgroundEntity/TextureOnBackgroundEntity.h"

class PawnPromotionButton : virtual public TextureOnBackgroundEntity
{
public:
	enum class Status
	{
		RELEASED,
		HOVERED,
	};

protected:

	PawnPromotionButton::Status status;

	std::string soundNameWhenPressed;

	std::string backgroundTextureNameWhenHovered;
	std::string soundNameWhenHovered;

	bool recentlyInteractedWith;

public:
	PawnPromotionButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, const std::string& backgroundTextureName, const std::string& soundNameWhenPressed, const std::string& backgroundTextureNameWhenHovered, const std::string& soundNameWhenHovered);
	~PawnPromotionButton();

	virtual void draw() override;
	virtual void update() override;

	inline bool getRecentlyInteractedWith() const { return this->recentlyInteractedWith; }
};