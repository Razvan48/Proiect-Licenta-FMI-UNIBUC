#include "DataBox.h"


#include "../../InputManager/InputManager.h"

DataBox::DataBox(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& textureName, const std::string& initialText)
	: Entity(centerPosX, centerPosY, width, height, rotateAngle)
	, TextEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, initialText)
	, TexturableEntity(centerPosX, centerPosY, width, height, rotateAngle, textureName)
	, TextOnBackgroundEntity(centerPosX, centerPosY, width, height, rotateAngle, color, fontName, initialText, textureName)
	, isSelected(false), initialText(initialText)
{

}

DataBox::~DataBox()
{

}

void DataBox::draw()
{
	TextOnBackgroundEntity::draw();
}

void DataBox::update()
{
	if (InputManager::get().isLeftMouseButtonReleased() && this->isInCompleteMouseCollision())
	{
		if (this->isSelected == false && this->text.empty())
		{
			this->text = this->initialText;
		}
		this->isSelected = true;
		this->color = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (InputManager::get().isLeftMouseButtonReleased() && this->isInCompleteOutsideMouseCollision())
	{
		this->isSelected = false;
		this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	if (this->isSelected)
	{
		if (InputManager::get().isKeyReleased(GLFW_KEY_BACKSPACE) && !this->text.empty())
		{
			this->text.pop_back();
		}
		else
		{
			for (char character = 'A'; character <= 'Z'; ++character)
			{
				if (InputManager::get().isKeyReleased(character))
				{
					this->text.push_back(character - 'A' + 'a');
				}
			}

			for (char character = '0'; character <= '9'; ++character)
			{
				if (InputManager::get().isKeyReleased(character))
				{
					this->text.push_back(character);
				}
			}

			if (InputManager::get().isKeyReleased(GLFW_KEY_PERIOD))
			{
				this->text.push_back('.');
			}

			if (InputManager::get().isKeyReleased(GLFW_KEY_SEMICOLON))
			{
				this->text.push_back(':');
			}

			/*
			if (InputManager::get().isKeyReleased(GLFW_KEY_SPACE))
			{
				this->text.push_back(' ');
			}
			*/
		}
	}
}