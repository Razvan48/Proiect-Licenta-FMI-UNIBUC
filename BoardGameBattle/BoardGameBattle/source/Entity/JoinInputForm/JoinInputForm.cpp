#include "JoinInputForm.h"

#include "../../InputManager/InputManager.h"
#include "../../AssetManager/AssetManager.h"

JoinInputForm::JoinInputForm(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
	, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed
	, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered
	, const std::string& soundNameWhenHovered, const Game::Mode& gameModeWhenPressed
	, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed
	, TextEntity playerNameTextEntity, TextEntity serverAddressTextEntity
	, DataBox playerNameDataBox, DataBox serverAddressDataBox
	, const std::string& soundNameWhenContentNotOk
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
	, playerNameTextEntity(playerNameTextEntity), serverAddressTextEntity(serverAddressTextEntity)
	, playerNameDataBox(playerNameDataBox), serverAddressDataBox(serverAddressDataBox)
	, soundNameWhenContentNotOk(soundNameWhenContentNotOk)
	, colorInputContentOk(colorInputContentOk), colorInputContentNotOk(colorInputContentNotOk)
	, playerNameContentOk(false), serverAddressContentOk(false)
{
	this->playerNameTextEntity.setColor(this->colorInputContentOk);
	this->serverAddressTextEntity.setColor(this->colorInputContentOk);
	this->playerNameDataBox.setColor(this->colorInputContentOk);
	this->serverAddressDataBox.setColor(this->colorInputContentOk);

	this->color = this->colorInputContentOk;
}

JoinInputForm::~JoinInputForm()
{

}

void JoinInputForm::draw()
{
	this->playerNameTextEntity.draw();
	this->playerNameDataBox.draw();

	this->serverAddressTextEntity.draw();
	this->serverAddressDataBox.draw();

	Button::draw();
}

void JoinInputForm::update()
{
	this->playerNameTextEntity.update();
	this->playerNameDataBox.update();

	this->serverAddressTextEntity.update();
	this->serverAddressDataBox.update();

	// Verificare continut casute de text
	// playerName
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
	// serverAddress
	// std::vector<int> posDots;
	// int posTwoDots = -1;
	this->serverAddressContentOk = true;
	for (int i = 0; i < this->serverAddressDataBox.getText().size() && this->serverAddressContentOk; ++i)
	{
		if (!
			(('0' <= this->serverAddressDataBox.getText()[i]
				&& this->serverAddressDataBox.getText()[i] <= '9')
				|| this->serverAddressDataBox.getText()[i] == '.'
				|| this->serverAddressDataBox.getText()[i] == ':')
			)
		{
			this->serverAddressContentOk = false;
		}

		/*
		if (this->serverAddressDataBox.getText()[i] == '.')
		{
			posDots.push_back(i);
		}
		if (this->serverAddressDataBox.getText()[i] == ':')
		{
			if (posTwoDots != -1)
			{
				this->serverAddressContentOk = false;
			}
			posTwoDots = i;
		}
		*/
	}
	/*
	if (this->serverAddressContentOk)
	{
		if (posTwoDots == -1 || posTwoDots == (int)this->serverAddressDataBox.getText().size() - 1)
		{
			this->serverAddressContentOk = false;
		}
		for (int i = 1; i < posDots.size() && this->serverAddressContentOk; ++i)
		{
			if (posDots[i] - posDots[i - 1] == 1)
			{
				this->serverAddressContentOk = false;
			}
		}
		if (posDots.empty() || posDots[0] == 0 || posDots.back() == (int)this->serverAddressDataBox.getText().size() - 1)
		{
			this->serverAddressContentOk = false;
		}
	}
	*/
	if (this->serverAddressDataBox.getText().empty())
		this->serverAddressContentOk = false;
	// Final verificare continut casute de text


	this->recentlyPressed = false;
	if (this->isInMouseCollision())
	{
		if (InputManager::get().isLeftMouseButtonReleased())
		{
			if (this->playerNameContentOk && this->serverAddressContentOk)
			{
				Game::get().setStatus(this->gameStatusWhenPressed);
				AssetManager::get().playSound(this->soundNameWhenPressed, false);
				this->recentlyPressed = true;
				Game::get().setMode(this->gameModeWhenPressed);
				Game::get().setColor(this->gameColorWhenPressed);
				Game::get().setMultiplayerStatus(this->gameMultiplayerStatusWhenPressed);

				this->playerNameTextEntity.setColor(this->colorInputContentOk);
				this->serverAddressTextEntity.setColor(this->colorInputContentOk);
			}
			else
			{
				if (!this->playerNameContentOk)
					this->playerNameTextEntity.setColor(this->colorInputContentNotOk);
				else
					this->playerNameTextEntity.setColor(this->colorInputContentOk);

				if (!this->serverAddressContentOk)
					this->serverAddressTextEntity.setColor(this->colorInputContentNotOk);
				else
					this->serverAddressTextEntity.setColor(this->colorInputContentOk);

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