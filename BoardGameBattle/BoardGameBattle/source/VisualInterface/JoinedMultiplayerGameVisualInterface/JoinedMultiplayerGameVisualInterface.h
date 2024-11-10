#pragma once

#include "../SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class JoinedMultiplayerGameVisualInterface : virtual public SingleplayerGameVisualInterface
{
protected:
	TextEntity serverConnectionStatusTextEntity;
	TextEntity opponentConnectionStatusTextEntity;
	TextEntity serverPortTextEntity;

	std::string playerName;
	std::string serverAddress;

public:
	JoinedMultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
		, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
		, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
		, TextEntity serverPortTextEntity);
	virtual ~JoinedMultiplayerGameVisualInterface();

	static JoinedMultiplayerGameVisualInterface& get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;

	void setServerStatus(bool statusOk);
	void setOpponentStatus(bool statusOk);
	inline void setServerPort(const std::string& serverPort) { this->serverPortTextEntity.setText(serverPort); }

	inline void setPlayerName(const std::string& playerName) { this->playerName = playerName; }
	inline void setServerAddress(const std::string& serverAddress) { this->serverAddress = serverAddress; }
};