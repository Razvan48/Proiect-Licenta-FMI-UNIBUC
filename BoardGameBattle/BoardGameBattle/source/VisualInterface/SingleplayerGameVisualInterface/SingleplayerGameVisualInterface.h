#pragma once

#include "../VisualInterface.h"
#include "../../Entity/TextEntity/TextEntity.h"

#include "../../Entity/Button/UndoMoveButton/UndoMoveButton.h"

#include <string>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SingleplayerGameVisualInterface : virtual public VisualInterface
{
protected:
	TextEntity turnLabelTextEntity;
	TextEntity playerNameLabelTextEntity;
	TextEntity opponentNameLabelTextEntity;

	TextEntity turnTextEntity;
	TextEntity playerNameTextEntity;
	TextEntity opponentNameTextEntity;

	UndoMoveButton undoMoveButton;

	TextEntity finalMessageTextEntity;

	bool displayFinalMessage;

	std::string boardStartSoundName;

public:
	enum class FinalMessage
	{
		WON,
		LOST,
		DRAW,
		IN_PAWN_PROMOTION_MENU,
		NOT_FINISHED,
	};

private:
	FinalMessage finalMessage;

public:
	SingleplayerGameVisualInterface(TexturableEntity backgroundEntity, bool respondsToEscapeKey
		, TextEntity turnLabelTextEntity, TextEntity playerNameLabelTextEntity, TextEntity opponentNameLabelTextEntity
		, TextEntity turnTextEntity
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

	void setFinalMessageTextEntity(SingleplayerGameVisualInterface::FinalMessage finalMessage);

	inline FinalMessage getFinalMessage() const { return this->finalMessage; }
};