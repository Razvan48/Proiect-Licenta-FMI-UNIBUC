#include "Entity.h"

#include "../InputManager/InputManager.h"

Entity::Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle)
	: posCenterX(posCenterX), posCenterY(posCenterY), width(width), height(height), rotateAngle(rotateAngle)
	, requestedDeletion(false), requestedToBeHidden(false)
{

}

bool Entity::isInMouseCollision() const
{
	float cursorPosX = InputManager::get().getCursorPosX();
	float cursorPosY = InputManager::get().getCursorPosY();

	return this->posCenterX - this->width / 2.0f < cursorPosX && cursorPosX < this->posCenterX + this->width / 2.0f
		&& this->posCenterY - this->height / 2.0f < cursorPosY && cursorPosY < this->posCenterY + this->height / 2.0f;
}

Entity::~Entity()
{

}