#include "Entity.h"

Entity::Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle)
	: posCenterX(posCenterX), posCenterY(posCenterY), width(width), height(height), rotateAngle(rotateAngle)
	, requestedDeletion(false), requestedToBeHidden(false)
{

}

Entity::~Entity()
{

}