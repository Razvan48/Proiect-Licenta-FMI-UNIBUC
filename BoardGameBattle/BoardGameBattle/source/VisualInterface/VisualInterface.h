#pragma once

#include <vector>
#include <memory>

#include "../Entity/Entity.h"

class VisualInterface
{
private:
	std::vector<std::shared_ptr<Entity>> entities;

public:
	VisualInterface();
	~VisualInterface();

	void draw();
	void update();
};