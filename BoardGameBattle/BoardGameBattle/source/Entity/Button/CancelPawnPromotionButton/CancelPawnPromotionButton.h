#pragma once

#include "../../TextOnBackgroundEntity/TextOnBackgroundEntity.h"

class CancelPawnPromotionButton : virtual public TextOnBackgroundEntity
{
public:
	enum class Status
	{
		RELEASED,
		HOVERED,
	};

protected:

	CancelPawnPromotionButton::Status status;

	std::string soundNameWhenPressed;

	std::string textureNameWhenHovered;
	glm::vec3 colorWhenHovered;
	std::string soundNameWhenHovered;

	bool recentlyInteractedWith;

public:
	CancelPawnPromotionButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
		, const std::string& soundNameWhenPressed, const std::string& textureNameWhenHovered
		, const glm::vec3& colorWhenHovered, const std::string& soundNameWhenHovered);
	virtual ~CancelPawnPromotionButton();

	virtual void draw() override;
	virtual void update() override;

	inline bool getRecentlyInteractedWith() const { return this->recentlyInteractedWith; }
};