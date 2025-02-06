#pragma once

#include "../TexturableEntity/TexturableEntity.h"

class TextureOnBackgroundEntity : virtual public TexturableEntity
{
protected:

	static const float TEXTURE_PADDING_0;
	static const float TEXTURE_PADDING_1;

	std::string backgroundTextureName;
	TexturableEntity backgroundEntity;

public:
	TextureOnBackgroundEntity(float centerPosX, float centerPosY, float width, float height, float rotateAngle, const std::string& textureName, const std::string& backgroundTextureName);
	~TextureOnBackgroundEntity();

	virtual void draw() override;
	virtual void update() override;
};