#include "SoundSetting.h"

#include "../../Game/Game.h"

SoundSetting::SoundSetting(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, Button buttonOn, Button buttonOff)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, buttonOn(buttonOn), buttonOff(buttonOff)
{

}

SoundSetting::~SoundSetting()
{

}

void SoundSetting::draw()
{
	TextEntity::draw();
	this->buttonOn.draw();
	this->buttonOff.draw();
}

void SoundSetting::update()
{
	TextEntity::update();
	this->buttonOn.update();
	this->buttonOff.update();

	if (this->buttonOn.getRecentlyPressed())
	{
		Game::get().setSoundEnabled(true);
		this->text = "Sound: ON";
		this->color = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (this->buttonOff.getRecentlyPressed())
	{
		Game::get().setSoundEnabled(false);
		this->text = "Sound: OFF";
		this->color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
}