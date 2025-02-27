#pragma once

#include "../TextEntity/TextEntity.h"
#include "../Button/Button.h"

class SoundSetting : virtual public TextEntity
{
protected:
	Button buttonOnOff;

public:
	SoundSetting(float centerPosX, float centerPosY, float width, float height, float rotateAngle
		, const glm::vec3& color, const std::string& fontName, const std::string& text, Button buttonOnOff);
	virtual ~SoundSetting();

	virtual void draw() override;
	virtual void update() override;
};