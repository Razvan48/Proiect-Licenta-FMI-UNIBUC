#pragma once


class SoundButton
{
protected:

public:
	SoundButton();
	virtual ~SoundButton();

	virtual void draw() = 0;
	virtual void update() = 0;
};