#pragma once

#include <string>

#include "../TexturableEntity/TexturableEntity.h"

class TextEntity : virtual public TexturableEntity
{
protected:
	std::string fontName;
	std::string text;

public:
	TextEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, bool requestedDeletion, bool requestedToBeHidden, const std::string& textureName, const glm::vec3& color, float textureBlendFactor, float backgroundBlendFactor, const std::string& fontName, const std::string& text);
	virtual ~TextEntity();

	virtual void draw() override;
	virtual void update() override;

	inline const std::string& getFontName() const { return this->fontName; }
	inline const std::string& getText() const { return this->text; }
	inline void setFont(const std::string& fontName) { this->fontName = fontName; }
	inline void setText(const std::string& text) { this->text = text; }
};