#pragma once

#include "../VisualInterface.h"
#include "../../Entity/Button/Button.h"

#include <memory>

class MultiplayerColorMenuVisualInterface : virtual public VisualInterface
{
protected:
	Button whiteButton;
	Button blackButton;

public:
	MultiplayerColorMenuVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	virtual ~MultiplayerColorMenuVisualInterface();

	static std::shared_ptr<MultiplayerColorMenuVisualInterface> get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;
};

