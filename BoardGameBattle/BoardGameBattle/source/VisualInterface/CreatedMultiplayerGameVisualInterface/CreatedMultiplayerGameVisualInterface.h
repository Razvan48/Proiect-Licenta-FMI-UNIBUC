#pragma once

#include "../SingleplayerGameVisualInterface/SingleplayerGameVisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

class CreatedMultiplayerGameVisualInterface : virtual public SingleplayerGameVisualInterface
{
protected:
	TextEntity serverConnectionStatusLabelTextEntity;
	TextEntity opponentConnectionStatusLabelTextEntity;
	TextEntity serverPortLabelTextEntity;

	TextEntity serverConnectionStatusTextEntity;
	TextEntity opponentConnectionStatusTextEntity;
	TextEntity serverPortTextEntity;

	std::string playerName;
	std::string serverAddress;
	std::string color;

	bool hasToSendBoardConfiguration;

	std::string pieceMoveSoundName;

public:
	CreatedMultiplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey
		, TextEntity turnLabelTextEntity, TextEntity playerNameLabelTextEntity, TextEntity opponentNameLabelTextEntity
		, TextEntity turnTextEntity
		, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity
		, TextEntity serverConnectionStatusLabelTextEntity, TextEntity opponentConnectionStatusLabelTextEntity, TextEntity serverPortLabelTextEntity
		, TextEntity serverConnectionStatusTextEntity, TextEntity opponentConnectionStatusTextEntity
		, TextEntity serverPortTextEntity);
	virtual ~CreatedMultiplayerGameVisualInterface();

	static std::shared_ptr<CreatedMultiplayerGameVisualInterface> get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;

	const std::string& getColor() const { return this->color; }

	inline void setServerPort(const std::string& serverPort) { this->serverPortTextEntity.setText(serverPort); }

	inline void setPlayerName(const std::string& playerName) { this->playerName = playerName; }
	inline void setServerAddress(const std::string& serverAddress) { this->serverAddress = serverAddress; }
	inline void setColor(const std::string& color) { this->color = color; }

	inline void setHasToSendBoardConfiguration(bool hasToSendBoardConfiguration) { this->hasToSendBoardConfiguration = hasToSendBoardConfiguration; }
};