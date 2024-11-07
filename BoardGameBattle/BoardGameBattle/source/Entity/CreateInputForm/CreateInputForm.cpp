#include "CreateInputForm.h"

#include "../../InputManager/InputManager.h"
#include "../../AssetManager/AssetManager.h"

CreateInputForm::CreateInputForm(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
	, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
	, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
	, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
	, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed
	, TextEntity playerNameTextEntity, DataBox playerNameDataBox, const std::string& soundNameWhenContentNotOk
	, const glm::vec3& colorInputContentOk, const glm::vec3& colorInputContentNotOk)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName)
	, Button(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName
		, gameStatusWhenPressed, soundNameWhenPressed
		, textureNameWhenHovered, colorWhenHovered
		, soundNameWhenHovered, gameModeWhenPressed
		, gameColorWhenPressed, gameMultiplayerStatusWhenPressed)
	, playerNameTextEntity(playerNameTextEntity), playerNameDataBox(playerNameDataBox)
	, soundNameWhenContentNotOk(soundNameWhenContentNotOk)
	, colorInputContentOk(colorInputContentOk), colorInputContentNotOk(colorInputContentNotOk)
	, playerNameContentOk(false)
{
	this->playerNameTextEntity.setColor(this->colorInputContentOk);
	this->playerNameDataBox.setColor(this->colorInputContentOk);

	this->color = this->colorInputContentOk;
}

CreateInputForm::~CreateInputForm()
{

}

void CreateInputForm::draw()
{
	this->playerNameTextEntity.draw();
	this->playerNameDataBox.draw();

	Button::draw();
}

void CreateInputForm::update()
{
	this->playerNameTextEntity.update();
	this->playerNameDataBox.update();

	this->playerNameContentOk = true;
	for (int i = 0; i < this->playerNameDataBox.getText().size() && this->playerNameContentOk; ++i)
	{
		if (!
			(('a' <= this->playerNameDataBox.getText()[i]
				&& this->playerNameDataBox.getText()[i] <= 'z')
				|| ('0' <= this->playerNameDataBox.getText()[i]
				&& this->playerNameDataBox.getText()[i] <= '9'))
			)
		{
			this->playerNameContentOk = false;
		}
	}
	if (this->playerNameDataBox.getText().empty())
		this->playerNameContentOk = false;


	this->recentlyPressed = false;
	if (this->isInMouseCollision())
	{
		if (InputManager::get().isLeftMouseButtonReleased())
		{
			if (this->playerNameContentOk)
			{
				Game::get().setStatus(this->gameStatusWhenPressed);
				AssetManager::get().playSound(this->soundNameWhenPressed, false);
				this->recentlyPressed = true;
				Game::get().setMode(this->gameModeWhenPressed);
				Game::get().setColor(this->gameColorWhenPressed);
				Game::get().setMultiplayerStatus(this->gameMultiplayerStatusWhenPressed);

				this->playerNameTextEntity.setColor(this->colorInputContentOk);

				Game::get().setPlayerNameForMultiplayer(this->playerNameDataBox.getText());
			}
			else
			{
				this->playerNameTextEntity.setColor(this->colorInputContentNotOk);

				AssetManager::get().playSound(this->soundNameWhenContentNotOk, false);
			}
		}
		else
		{
			if (this->status == Button::Status::RELEASED)
			{
				AssetManager::get().playSound(this->soundNameWhenHovered, false);
				this->text.push_back('<');
			}
			this->status = Button::Status::HOVERED;
		}
	}
	else
	{
		if (this->status == Button::Status::HOVERED)
		{
			this->text.pop_back();
		}
		this->status = Button::Status::RELEASED;
	}
}