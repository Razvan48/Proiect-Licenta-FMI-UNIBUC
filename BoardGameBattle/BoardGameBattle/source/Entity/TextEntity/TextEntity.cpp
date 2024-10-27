#include "TextEntity.h"

#include "../../Renderer/Renderer.h"

TextEntity::TextEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor)
	, text(text), fontName(fontName)
{

}

TextEntity::~TextEntity()
{

}

void TextEntity::draw()
{
	TexturableEntity::draw();

	Renderer::get().drawText(
		this->posCenterX,
		this->posCenterY,
		this->width,
		this->rotateAngle,
		this->fontName,
		this->text,
		this->color,
		this->textureBlendFactor,
		this->backgroundBlendFactor
	);
}

void TextEntity::update()
{

}