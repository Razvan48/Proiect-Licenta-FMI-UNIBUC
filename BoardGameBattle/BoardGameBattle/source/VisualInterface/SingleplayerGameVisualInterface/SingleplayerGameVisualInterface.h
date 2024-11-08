#pragma once

#include "../VisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

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

public:
	SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey, TextEntity turnTextEntity
		, TextEntity playerNameTextEntity, TextEntity opponentNameTextEntity, TextEntity finalMessageTextEntity);
	~SingleplayerGameVisualInterface();

	virtual void onVisualInterfaceLoad() override;
	virtual void draw() override;
	virtual void update() override;

	inline const TextEntity& getTurnTextEntity() const { return this->turnTextEntity; }
	inline const TextEntity& getPlayerNameTextEntity() const { return this->playerNameTextEntity; }
	inline const TextEntity& getOpponentNameTextEntity() const { return this->opponentNameTextEntity; }
	inline const TextEntity& getFinalMessageTextEntity() const { return this->finalMessageTextEntity; }

	void setTurn(bool whiteTurn);
	void setPlayerName(const std::string& playerName);
	void setOpponentName(const std::string& opponentName);
	inline void hideFinalMessage() { this->finalMessageTextEntity.setRequestedToBeHidden(true); }
	void setFinalMessage(bool hasWon);
};