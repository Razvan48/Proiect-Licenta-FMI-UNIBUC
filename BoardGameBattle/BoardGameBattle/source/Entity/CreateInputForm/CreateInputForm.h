#pragma once

#include "../Button/Button.h"
#include "../TextEntity/TextEntity.h"
#include "../DataBox/DataBox.h"

class CreateInputForm : virtual public Button
{
protected:
	TextEntity playerNameTextEntity;
	DataBox playerNameDataBox;

	bool playerNameContentOk;
	std::string soundNameWhenContentNotOk;

	glm::vec3 colorInputContentOk;
	glm::vec3 colorInputContentNotOk;

public:
	CreateInputForm(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
		, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
		, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
		, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
		, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed
		, TextEntity playerNameTextEntity, DataBox playerNameDataBox, const std::string& soundNameWhenContentNotOk
		, const glm::vec3& colorInputContentOk, const glm::vec3& colorInputContentNotOk);
	virtual ~CreateInputForm();

	virtual void draw() override;
	virtual void update() override;
	void initialize();
};