#pragma once

#include "../VisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

#include <string>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SingleplayerGameVisualInterface : virtual public VisualInterface
{
protected:
	TextEntity turnTextEntity;
	TextEntity playerNameTextEntity;
	TextEntity opponentNameTextEntity;
	TextEntity finalMessageTextEntity;

	bool displayFinalMessage;

	std::string boardStartSoundName;

public:
	SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
		, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity);
	virtual ~SingleplayerGameVisualInterface();

	static std::shared_ptr<SingleplayerGameVisualInterface> get();

	virtual void initialize() override;
	virtual void draw() override;
	virtual void update() override;

	inline const TextEntity& getTurnTextEntity() const { return this->turnTextEntity; }
	inline const TextEntity& getPlayerNameTextEntity() const { return this->playerNameTextEntity; }
	inline const TextEntity& getOpponentNameTextEntity() const { return this->opponentNameTextEntity; }
	inline const TextEntity& getFinalMessageTextEntity() const { return this->finalMessageTextEntity; }

	void setTurn(bool whiteTurn);
	void setPlayerName(const std::string& playerName);
	void setOpponentName(const std::string& opponentName);
	inline void setDisplayFinalMessage(bool displayFinalMessage) { this->displayFinalMessage = displayFinalMessage; }
	void setFinalMessage(bool hasWon);
};