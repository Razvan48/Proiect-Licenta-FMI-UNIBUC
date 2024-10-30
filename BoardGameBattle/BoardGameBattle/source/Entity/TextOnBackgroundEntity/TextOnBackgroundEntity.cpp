#include "TextOnBackgroundEntity.h"

#include "../../Renderer/Renderer.h"


TextOnBackgroundEntity::TextOnBackgroundEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
{

}

TextOnBackgroundEntity::~TextOnBackgroundEntity()
{

}

void TextOnBackgroundEntity::draw()
{
	TexturableEntity::draw();

	TextEntity::draw();
}

void TextOnBackgroundEntity::update()
{

}