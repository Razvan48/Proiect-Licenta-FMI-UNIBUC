#pragma once

#include <vector>
#include <memory>

#include "../Entity/Entity.h"
#include "../Entity/TexturableEntity/TexturableEntity.h"

class VisualInterface
{
protected:
	TexturableEntity backgroundEntity;
	std::vector<std::shared_ptr<Entity>> entities;
	bool respondsToEscapeKey;

public:
	VisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	virtual ~VisualInterface();

	virtual void initialize();
	virtual void draw();
	virtual void update();

	inline void addEntity(const std::shared_ptr<Entity>& entity) { this->entities.push_back(entity); }
};