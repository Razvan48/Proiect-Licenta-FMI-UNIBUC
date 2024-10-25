#pragma once

class Entity
{
protected:
	float posCenterX;
	float posCenterY;
	float width;
	float height;
	float rotateAngle;

public:
	Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle);
	~Entity();

	virtual void draw() = 0;
	virtual void update() = 0;
};