#pragma once


class InputButton
{
protected:

public:
	InputButton();
	virtual ~InputButton();

	virtual void draw() = 0;
	virtual void update() = 0;
};