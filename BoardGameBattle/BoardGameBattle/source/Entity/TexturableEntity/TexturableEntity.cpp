#include "TexturableEntity.h"

#include "../../Renderer/Renderer.h"

TexturableEntity::TexturableEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden)
	, textureName(textureName), color(color), textureBlendFactor(textureBlendFactor), backgroundBlendFactor(backgroundBlendFactor)
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
		this->textureBlendFactor,
		this->backgroundBlendFactor,
		false
	);
}

void TexturableEntity::update()
{

}