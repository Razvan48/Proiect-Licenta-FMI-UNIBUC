#pragma once

#include "../TextEntity/TextEntity.h"
#include "../Button/Button.h"

class SoundButton : virtual public TextEntity
{
protected:
	Button buttonOn;
	Button buttonOff;

public:
	SoundButton(float centerPosX, float centerPosY, float width, float height, float rotateAngle
		, const glm::vec3& color, const std::string& fontName, const std::string& text, Button buttonOn, Button buttonOff);
	virtual ~SoundButton();

	virtual void draw();
	virtual void update();
};