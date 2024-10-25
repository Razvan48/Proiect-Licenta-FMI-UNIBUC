#include "TexturableEntity.h"

#include "../../Renderer/Renderer.h"

TexturableEntity::TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), float blendFactor = 0.0f)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, textureName(textureName), color(color), blendFactor(blendFactor)
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
		this->textureName,
		this->color,
		this->blendFactor
	);
}

void TexturableEntity::update()
{

}