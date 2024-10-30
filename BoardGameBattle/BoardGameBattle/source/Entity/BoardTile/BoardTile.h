#pragma once


#include "../TexturableEntity/TexturableEntity.h"

class BoardTile : virtual public TexturableEntity
{
protected:

public:
	BoardTile(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName);
	virtual ~BoardTile();

	virtual void draw() override;
	virtual void update() override;
};