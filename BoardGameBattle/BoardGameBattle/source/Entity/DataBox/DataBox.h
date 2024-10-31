#pragma once

#include "../TextOnBackgroundEntity/TextOnBackgroundEntity.h"

#include <string>

class DataBox : virtual public TextOnBackgroundEntity
{
protected:
	bool isSelected;
	std::string initialText;

public:
	DataBox(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName, const std::string& initialText);
	virtual ~DataBox();

	virtual void draw() override;
	virtual void update() override;
};