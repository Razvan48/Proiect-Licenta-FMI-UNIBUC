#include "InGameVisualInterface.h"

InGameVisualInterface::InGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey)
	: VisualInterface(backgroundEntity, respondsToEscapeKey)
{

}

InGameVisualInterface::~InGameVisualInterface()
{

}

void InGameVisualInterface::draw()
{
	VisualInterface::draw();
}

void InGameVisualInterface::update()
{
	VisualInterface::update();
}