#include "BoardTile.h"



BoardTile::BoardTile(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
{

}

BoardTile::~BoardTile()
{

}

void BoardTile::draw()
{
	TexturableEntity::draw();
}

void BoardTile::update()
{

}