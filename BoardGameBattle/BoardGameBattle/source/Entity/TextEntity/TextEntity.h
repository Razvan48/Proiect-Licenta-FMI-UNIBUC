#pragma once

#include <string>

#include "../Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TextEntity : virtual public Entity
{
protected:
	glm::vec3 color;
	std::string fontName;
	std::string text;

	static const float TEXT_PADDING_0;
	static const float TEXT_PADDING_1;

public:
	TextEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const glm::vec3& color, const std::string& fontName, const std::string& text);
	virtual ~TextEntity();

	virtual void draw() override;
	virtual void update() override;

	inline const std::string& getFontName() const { return this->fontName; }
	inline const std::string& getText() const { return this->text; }
	inline void setFont(const std::string& fontName) { this->fontName = fontName; }
	inline void setText(const std::string& text) { this->text = text; }

	inline void setColor(const glm::vec3& color) { this->color = color; }
};