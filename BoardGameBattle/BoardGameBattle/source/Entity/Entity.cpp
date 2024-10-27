#include "Entity.h"

Entity::Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden)
	: posCenterX(posCenterX), posCenterY(posCenterY), width(width), height(height), rotateAngle(rotateAngle), requestedDeletion(requestedDeletion), requestedToBeHidden(requestedToBeHidden)
{

}

Entity::~Entity()
{

}