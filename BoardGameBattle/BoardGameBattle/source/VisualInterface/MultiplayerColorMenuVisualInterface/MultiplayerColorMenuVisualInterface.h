#pragma once

#include "../VisualInterface.h"

class MultiplayerColorMenuVisualInterface : virtual public VisualInterface
{
protected:

public:
	MultiplayerColorMenuVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	virtual ~MultiplayerColorMenuVisualInterface();

	static MultiplayerColorMenuVisualInterface& get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;
};

