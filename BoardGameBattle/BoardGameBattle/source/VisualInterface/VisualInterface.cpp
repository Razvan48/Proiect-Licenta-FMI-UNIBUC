#include "VisualInterface.h"

VisualInterface::VisualInterface()
{

}

VisualInterface::~VisualInterface()
{

}

void VisualInterface::draw()
{
	for (int i = 0; i < this->entities.size(); ++i)
	{
		if (this->entities[i]->getRequestedToBeHidden())
			continue;

		this->entities[i]->draw();
	}
}

void VisualInterface::update()
{
	for (int i = 0; i < this->entities.size(); ++i)
	{
		if (this->entities[i]->getRequestedDeletion())
		{
			std::swap(this->entities[i], this->entities.back());
			this->entities.pop_back();
			--i;
		}
	}

	for (int i = 0; i < this->entities.size(); ++i)
	{
		this->entities[i]->update();
	}
}