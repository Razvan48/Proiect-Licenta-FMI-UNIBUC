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
	glm::vec3 color;
	float textureBlendFactor;
	float backgroundBlendFactor;


public:
	TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor);
	virtual ~TexturableEntity();

	virtual void draw() override;
	virtual void update() override;
};