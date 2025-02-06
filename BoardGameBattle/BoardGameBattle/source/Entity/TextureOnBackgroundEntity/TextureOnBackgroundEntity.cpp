#include "TextureOnBackgroundEntity.h"

#include "../../Renderer/Renderer.h"

TextureOnBackgroundEntity::TextureOnBackgroundEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, const std::string& backgroundTextureName)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, backgroundTextureName(backgroundTextureName)
	, backgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, backgroundTextureName)
{

}

const float TextureOnBackgroundEntity::TEXTURE_PADDING_0 = 0.95f;
const float TextureOnBackgroundEntity::TEXTURE_PADDING_1 = 0.75f;

TextureOnBackgroundEntity::~TextureOnBackgroundEntity()
{

}

void TextureOnBackgroundEntity::draw()
{
	this->backgroundEntity.draw();

	Renderer::get().draw(
		this->posCenterX,
		this->posCenterY,
		TextureOnBackgroundEntity::TEXTURE_PADDING_1 * this->width,
		TextureOnBackgroundEntity::TEXTURE_PADDING_1 * this->height,
		this->rotateAngle,
		this->textureName
	);
}

void TextureOnBackgroundEntity::update()
{
	this->backgroundEntity.update();

	TexturableEntity::update();
}