#include "GamePiece.h"

GamePiece::GamePiece(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, requestedDeletion, requestedToBeHidden, textureName, color, textureBlendFactor, backgroundBlendFactor)
{

}

GamePiece::~GamePiece()
{

}

void GamePiece::draw()
{
	TexturableEntity::draw();
}

void GamePiece::update()
{

}