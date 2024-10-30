#include "TexturableEntity.h"

#include "../../Renderer/Renderer.h"

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
	Renderer::get().draw(
		this->posCenterX,
		this->posCenterY,
		this->width,
		this->height,
		this->rotateAngle,
		this->textureName
	);
}

void TexturableEntity::update()
{

}