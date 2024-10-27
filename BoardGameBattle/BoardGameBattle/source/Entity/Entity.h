#pragma once

class Entity
{
protected:
	float posCenterX;
	float posCenterY;
	float width;
	float height;
	float rotateAngle;

	bool requestedDeletion;
	bool requestedToBeHidden;

public:
	Entity(float posCenterX, float posCenterY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden);
	virtual ~Entity();

	virtual void draw() = 0;
	virtual void update() = 0;

	inline bool getRequestedDeletion() const { return this->requestedDeletion; }
	inline void setRequestedDeletion(bool requestedDeletion) { this->requestedDeletion = requestedDeletion; }

	inline bool getRequestedToBeHidden() const { return this->requestedToBeHidden; }
	inline void setRequestedToBeHidden(bool requestedToBeHidden) { this->requestedToBeHidden = requestedToBeHidden; }
};