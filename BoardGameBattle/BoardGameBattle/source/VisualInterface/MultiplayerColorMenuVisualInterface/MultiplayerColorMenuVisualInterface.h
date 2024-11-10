#pragma once

#include "../VisualInterface.h"
#include "../../Entity/Button/Button.h"

class MultiplayerColorMenuVisualInterface : virtual public VisualInterface
{
protected:
	Button whiteButton;
	Button blackButton;

public:
	MultiplayerColorMenuVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	virtual ~MultiplayerColorMenuVisualInterface();

	static MultiplayerColorMenuVisualInterface& get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;
};

