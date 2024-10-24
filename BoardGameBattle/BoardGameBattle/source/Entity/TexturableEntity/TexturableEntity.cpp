#include "TexturableEntity.h"

TexturableEntity::TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, textureName(textureName)
{

}

TexturableEntity::~TexturableEntity()
{

}

void TexturableEntity::draw()
{

}

void TexturableEntity::update()
{

}