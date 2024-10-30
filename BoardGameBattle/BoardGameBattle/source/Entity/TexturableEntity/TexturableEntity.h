#pragma once

#include "../Entity.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TexturableEntity : virtual public Entity
{
protected:
	std::string textureName;

public:
	TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName);
	virtual ~TexturableEntity();

	virtual void draw() override;
	virtual void update() override;
};