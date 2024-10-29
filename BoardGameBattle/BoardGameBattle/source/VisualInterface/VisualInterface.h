#pragma once

#include <vector>
#include <memory>

#include "../Entity/Entity.h"
#include "../Entity/TexturableEntity/TexturableEntity.h"

class VisualInterface
{
private:
	TexturableEntity backgroundEntity;
	std::vector<std::shared_ptr<Entity>> entities;
	bool respondsToEscapeKey;

public:
	VisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	~VisualInterface();

	void draw();
	void update();

	inline void addEntity(const std::shared_ptr<Entity>& entity) { this->entities.push_back(entity); }
};