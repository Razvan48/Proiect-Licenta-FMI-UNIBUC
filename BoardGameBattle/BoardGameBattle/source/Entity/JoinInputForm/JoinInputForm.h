#pragma once

#include "../Button/Button.h"
#include "../TextEntity/TextEntity.h"
#include "../DataBox/DataBox.h"

class JoinInputForm : virtual public Button
{
protected:
	TextEntity playerNameTextEntity;
	TextEntity serverAddressTextEntity;
	DataBox playerNameDataBox;
	DataBox serverAddressDataBox;

	bool playerNameContentOk;
	std::string soundNameWhenContentNotOk;

	bool serverAddressContentOk;

	glm::vec3 colorInputContentOk;
	glm::vec3 colorInputContentNotOk;

public:
	JoinInputForm(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
		, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
		, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
		, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
		, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed
		, TextEntity playerNameTextEntity, TextEntity serverAddressTextEntity
		, DataBox playerNameDataBox, DataBox serverAddressDataBox
		, const std::string& soundNameWhenContentNotOk
		, const glm::vec3& colorInputContentOk, const glm::vec3& colorInputContentNotOk);
	virtual ~JoinInputForm();

	virtual void draw() override;
	virtual void update() override;
};