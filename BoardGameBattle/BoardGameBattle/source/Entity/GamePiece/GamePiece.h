#pragma once

#include "../TexturableEntity/TexturableEntity.h"

class GamePiece : virtual public TexturableEntity
{
protected:

public:
	GamePiece(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor);
	virtual ~GamePiece();

	virtual void draw() override;
	virtual void update() override;
};