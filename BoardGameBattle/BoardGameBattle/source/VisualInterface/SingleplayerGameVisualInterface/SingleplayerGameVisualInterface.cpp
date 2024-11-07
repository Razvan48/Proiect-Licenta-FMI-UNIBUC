#include "SingleplayerGameVisualInterface.h"

SingleplayerGameVisualInterface::SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
{

}

SingleplayerGameVisualInterface::~SingleplayerGameVisualInterface()
{

}

void SingleplayerGameVisualInterface::draw()
{
	VisualInterface::draw();
}

void SingleplayerGameVisualInterface::update()
{
	VisualInterface::update();
}