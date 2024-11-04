#pragma once

#include "../VisualInterface.h"

#include "../../Entity/TexturableEntity/TexturableEntity.h"

class InGameVisualInterface : virtual public VisualInterface
{
protected:

public:
	InGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	~InGameVisualInterface();

	virtual void draw() override;
	virtual void update() override;
};