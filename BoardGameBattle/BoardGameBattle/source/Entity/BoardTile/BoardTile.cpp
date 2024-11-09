#include "BoardTile.h"

#include "../../Renderer/Renderer.h"

BoardTile::BoardTile(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, bool isSelected, const std::string& textureNameWhenSelected)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, isSelected(isSelected), textureNameWhenSelected(textureNameWhenSelected)
{

}

BoardTile::~BoardTile()
{

}

void BoardTile::draw()
{
	if (this->isSelected)
	{
		Renderer::get().draw(
			this->posCenterX,
			this->posCenterY,
			this->width,
			this->height,
			this->rotateAngle,
			this->textureNameWhenSelected
		);
	}
	else
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
}

void BoardTile::update()
{

}