#include "VisualInterface.h"

#include "../InputManager/InputManager.h"
#include "../Game/Game.h"

VisualInterface::VisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey)
	: backgroundEntity(backgroundEntity), respondsToEscapeKey(respondsToEscapeKey)
{

}

VisualInterface::~VisualInterface()
{

}

void VisualInterface::draw()
{
	this->backgroundEntity.draw(); // mai intai fundalul

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

	if (this->respondsToEscapeKey && InputManager::get().isKeyReleased(GLFW_KEY_ESCAPE))
	{
		Game::get().setStatus(Game::Status::EXITING);
	}
}