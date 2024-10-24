#pragma once

#include "../Entity.h"

#include <string>

class TexturableEntity : Entity
{
private:
	std::string textureName;

public:
	TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName);
	~TexturableEntity();

	virtual void draw() override;
	virtual void update() override;
};