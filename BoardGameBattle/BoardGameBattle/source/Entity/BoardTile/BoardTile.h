#pragma once


#include "../TexturableEntity/TexturableEntity.h"

#include <string>

class BoardTile : virtual public TexturableEntity
{
protected:
	bool isSelected;
	std::string textureNameWhenSelected;

public:
	BoardTile(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName
	, bool isSelected, const std::string& textureNameWhenSelected);
	virtual ~BoardTile();

	virtual void draw() override;
	virtual void update() override;

	inline bool getIsSelected() const { return this->isSelected; }
	inline void setIsSelected(bool isSelected) { this->isSelected = isSelected; }
};