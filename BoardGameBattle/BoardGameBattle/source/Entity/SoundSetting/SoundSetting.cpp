#include "SoundSetting.h"

#include "../../AssetManager/AssetManager.h"

SoundSetting::SoundSetting(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, Button buttonOnOff)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, buttonOnOff(buttonOnOff)
{

}

SoundSetting::~SoundSetting()
{

}

void SoundSetting::draw()
{
	TextEntity::draw();
	this->buttonOnOff.draw();
}

void SoundSetting::update()
{
	TextEntity::update();
	this->buttonOnOff.update();

	if (this->buttonOnOff.getRecentlyInteractedWith())
	{
		if (AssetManager::get().getSoundEnabled())
		{
			AssetManager::get().setSoundEnabled(false);
			this->text = "Sound: OFF";
			this->color = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			AssetManager::get().setSoundEnabled(true);
			this->text = "Sound: ON";
			this->color = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}
}