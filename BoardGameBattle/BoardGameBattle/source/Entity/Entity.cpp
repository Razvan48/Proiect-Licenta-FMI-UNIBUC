#include "Entity.h"

#include "../InputManager/InputManager.h"

Entity::Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle)
	: posCenterX(posCenterX), posCenterY(posCenterY), width(width), height(height), rotateAngle(rotateAngle)
	, requestedDeletion(false)
{

}

bool Entity::isInMouseCollision() const
{
	float cursorPosX = InputManager::get().getCursorPosX();
	float cursorPosY = InputManager::get().getCursorPosY();

	return this->posCenterX - this->width / 2.0f < cursorPosX && cursorPosX < this->posCenterX + this->width / 2.0f
		&& this->posCenterY - this->height / 2.0f < cursorPosY && cursorPosY < this->posCenterY + this->height / 2.0f;
}

bool Entity::isInMouseLastPressedCollision() const
{
	float cursorPosX = InputManager::get().getCursorLastPressedLeftMousePosX();
	float cursorPosY = InputManager::get().getCursorLastPressedLeftMousePosY();

	return this->posCenterX - this->width / 2.0f < cursorPosX && cursorPosX < this->posCenterX + this->width / 2.0f
		&& this->posCenterY - this->height / 2.0f < cursorPosY && cursorPosY < this->posCenterY + this->height / 2.0f;
}

bool Entity::isInCompleteMouseCollision() const
{
	return this->isInMouseCollision() && this->isInMouseLastPressedCollision();
}

bool Entity::isInCompleteOutsideMouseCollision() const
{
	return !this->isInMouseCollision() && !this->isInMouseLastPressedCollision();
}

Entity::~Entity()
{

}