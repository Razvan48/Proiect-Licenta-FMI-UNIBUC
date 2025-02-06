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

	inline std::string getTextureName() const { return this->textureName; }
	inline std::string getBackgroundTextureName() const { return this->backgroundTextureName; }

	inline void setTextureName(const std::string& textureName) { this->textureName = textureName; }
	inline void setBackgroundTextureName(const std::string& backgroundTextureName) { this->backgroundTextureName = backgroundTextureName; }
};