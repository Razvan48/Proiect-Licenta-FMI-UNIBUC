#pragma once

#include "../SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CreatedMultiplayerGameVisualInterface : virtual public SingleplayerGameVisualInterface
{
protected:
	TextEntity serverConnectionStatusTextEntity;
	TextEntity opponentConnectionStatusTextEntity;
	TextEntity serverPortTextEntity;

public:
	CreatedMultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
		, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
		, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
		, TextEntity serverPortTextEntity);
	virtual ~CreatedMultiplayerGameVisualInterface();

	static CreatedMultiplayerGameVisualInterface& get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;

	void setServerStatus(bool statusOk);
	void setOpponentStatus(bool statusOk);
	inline void setServerPort(const std::string& serverPort) { this->serverPortTextEntity.setText(serverPort); }
};