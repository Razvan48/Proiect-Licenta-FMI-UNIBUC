#pragma once

#include "../VisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"


class SingleplayerGameVisualInterface : public virtual VisualInterface
{
protected:
	//TextEntity turnTextEntity;
	//TextEntity playerNameTextEntity;
	//TextEntity opponentNameTextEntity;
	//TextEntity finalMessageTextEntity;

public:
	SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey);
	~SingleplayerGameVisualInterface();

	virtual void draw() override;
	virtual void update() override;
};