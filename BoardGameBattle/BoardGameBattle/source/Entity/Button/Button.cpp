#include "Button.h"

#include "../../Renderer/Renderer.h"
#include "../../InputManager/InputManager.h"
#include "../../AssetManager/AssetManager.h"

Button::Button(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text, const std::string& textureName
	, const Game::Status& gameStatusWhenPressed, const std::string& soundNameWhenPressed, const std::string& textureNameWhenHovered, const glm::vec3 colorWhenHovered, const std::string& soundNameWhenHovered
	, const Game::Mode& gameModeWhenPressed
	, const Game::Color& gameColorWhenPressed, const Game::MultiplayerStatus& gameMultiplayerStatusWhenPressed)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, text, textureName)
	, status(Button::Status::RELEASED)
	, gameStatusWhenPressed(gameStatusWhenPressed), soundNameWhenPressed(soundNameWhenPressed)
	, textureNameWhenHovered(textureNameWhenHovered), colorWhenHovered(colorWhenHovered)
	, soundNameWhenHovered(soundNameWhenHovered), recentlyPressed(false)
	, gameModeWhenPressed(gameModeWhenPressed), gameColorWhenPressed(gameColorWhenPressed), gameMultiplayerStatusWhenPressed(gameMultiplayerStatusWhenPressed)
{

}

Button::~Button()
{

}

void Button::draw()
{
	std::string currentTexture = "";
	if (this->status == Button::Status::RELEASED)
	{
		currentTexture = this->textureName;
	}
	else // if (this->status == Button::Status::HOVERED)
	{
		currentTexture = this->textureNameWhenHovered;
	}

	Renderer::get().draw( // background
		this->posCenterX,
		this->posCenterY,
		this->width,
		this->height,
		this->rotateAngle,
		currentTexture
	);

	/*
	Renderer::get().drawText( // contur
		this->posCenterX,
		this->posCenterY,
		TextEntity::TEXT_PADDING_0 * this->width,
		TextEntity::TEXT_PADDING_0 * this->height,
		this->rotateAngle,
		this->fontName,
		this->text,
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	*/

	glm::vec3 currentColor = glm::vec3(1.0f, 1.0f, 1.0f);
	if (this->status == Button::Status::RELEASED)
	{
		currentColor = this->color;
	}
	else // if (this->status == Button::Status::HOVERED)
	{
		currentColor = this->colorWhenHovered;
	}
	Renderer::get().drawText(
		this->posCenterX,
		this->posCenterY,
		TextEntity::TEXT_PADDING_1 * this->width,
		TextEntity::TEXT_PADDING_1 * this->height,
		this->rotateAngle,
		this->fontName,
		this->text,
		currentColor
	);
}

void Button::update()
{
	this->recentlyPressed = false;

	if (this->isInMouseCollision())
	{
		if (InputManager::get().isLeftMouseButtonReleased())
		{
			Game::get().setStatus(this->gameStatusWhenPressed);
			AssetManager::get().playSound(this->soundNameWhenPressed, false);
			this->recentlyPressed = true;
			Game::get().setMode(this->gameModeWhenPressed);
			Game::get().setColor(this->gameColorWhenPressed);
			Game::get().setMultiplayerStatus(this->gameMultiplayerStatusWhenPressed);
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