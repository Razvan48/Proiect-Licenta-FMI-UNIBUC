#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../Game/Game.h"

AssetManager::AssetManager()
	: FONT_SIZE(48), soundEnabled(true)
{
	stbi_set_flip_vertically_on_load(true);

	this->soundEngine = irrklang::createIrrKlangDevice();

	if (FT_Init_FreeType(&this->freeType))
	{
		std::cout << "Error: Could not init FreeType Library" << std::endl;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

AssetManager::~AssetManager()
{
	FT_Done_FreeType(this->freeType);
	this->soundEngine->drop();
}

AssetManager& AssetManager::get()
{
	static AssetManager instance;
	return instance;
}

void AssetManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	if (this->textures.find(textureName) != this->textures.end())
		std::cout << "Warning: Texture " + textureName + " already loaded" << std::endl;
	else
		this->textures[textureName] = AssetManager::Texture();

	glGenTextures(1, &this->textures[textureName].texture);
	glBindTexture(GL_TEXTURE_2D, this->textures[textureName].texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* textureData = stbi_load(texturePath.c_str(),
		&this->textures[textureName].width,
		&this->textures[textureName].height,
		&this->textures[textureName].nrChannels,
		0);

	if (!textureData)
	{
		std::cout << "Error: Failed to load texture " + textureName + " at path : " << texturePath << std::endl;
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textures[textureName].width, this->textures[textureName].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(textureData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void AssetManager::loadSound(const std::string& soundName, const std::string& soundPath)
{
	if (this->sounds.find(soundName) != this->sounds.end())
		std::cout << "Warning: Sound " + soundName + " already loaded" << std::endl;
	else
		this->sounds[soundName] = soundPath;
}

void AssetManager::loadFont(const std::string& fontName, const std::string& fontPath)
{
	FT_Face freeTypeFace;

	if (FT_New_Face(this->freeType, fontPath.c_str(), 0, &freeTypeFace)) // 0 = un singur tip de font, nu avem bold/italic/etc.
	{
		std::cout << "Error: Failed to load font " << fontName << " from path " << fontPath << std::endl;
	}
	else
	{
		if (this->fonts.find(fontName) != this->fonts.end())
			std::cout << "Warning: Font " + fontName + " already loaded" << std::endl;
		else
			this->fonts[fontName] = std::vector<AssetManager::Character>();

		FT_Set_Pixel_Sizes(freeTypeFace, 0, this->FONT_SIZE); // Setam inaltimea, latimea se seteaza automat

		for (unsigned char character = 0; character < 128; character++) // Cu primele 128 de caractere din tabela ascii putem desena cam orice text
		{
			if (FT_Load_Char(freeTypeFace, character, FT_LOAD_RENDER)) // Incarcare caracter
			{
				std::cout << "Error: Failed to load Glyph for " << character << std::endl;
				continue;
			}

			// Generare textura
			this->fonts[fontName].emplace_back();

			this->fonts[fontName].back().size = glm::ivec2(freeTypeFace->glyph->bitmap.width, freeTypeFace->glyph->bitmap.rows);
			this->fonts[fontName].back().bearing = glm::ivec2(freeTypeFace->glyph->bitmap_left, freeTypeFace->glyph->bitmap_top);
			this->fonts[fontName].back().advance = freeTypeFace->glyph->advance.x;

			glGenTextures(1, &this->fonts[fontName].back().texture);
			glBindTexture(GL_TEXTURE_2D, this->fonts[fontName].back().texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->fonts[fontName].back().size[0],
				this->fonts[fontName].back().size[1], 0, GL_RED, GL_UNSIGNED_BYTE,
				freeTypeFace->glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	FT_Done_Face(freeTypeFace);
}

void AssetManager::loadResources()
{
	this->loadTexture("mainMenuBackgroundTexture", "assets/textures/mainMenuBackgroundTexture.png");
	this->loadTexture("settingsMenuBackgroundTexture", "assets/textures/settingsMenuBackgroundTexture.png");
	this->loadTexture("modeMenuBackgroundTexture", "assets/textures/modeMenuBackgroundTexture.png");
	this->loadTexture("singleplayerColorMenuBackgroundTexture", "assets/textures/singleplayerColorMenuBackgroundTexture.png");
	this->loadTexture("multiplayerColorMenuBackgroundTexture", "assets/textures/multiplayerColorMenuBackgroundTexture.png");
	this->loadTexture("multiplayerMenuBackgroundTexture", "assets/textures/multiplayerMenuBackgroundTexture.png");
	this->loadTexture("inputCreateMenuBackgroundTexture", "assets/textures/inputCreateMenuBackgroundTexture.png");
	this->loadTexture("inputJoinMenuBackgroundTexture", "assets/textures/inputJoinMenuBackgroundTexture.png");
	this->loadTexture("singleplayerGameBackgroundTexture", "assets/textures/singleplayerGameBackgroundTexture.png");
	this->loadTexture("createdMultiplayerGameBackgroundTexture", "assets/textures/createdMultiplayerGameBackgroundTexture.png");
	this->loadTexture("joinedMultiplayerGameBackgroundTexture", "assets/textures/joinedMultiplayerGameBackgroundTexture.png");

	this->loadTexture("whitePawnTexture", "assets/textures/whitePawnTexture.png");
	this->loadTexture("whiteRookTexture", "assets/textures/whiteRookTexture.png");
	this->loadTexture("whiteKnightTexture", "assets/textures/whiteKnightTexture.png");
	this->loadTexture("whiteBishopTexture", "assets/textures/whiteBishopTexture.png");
	this->loadTexture("whiteQueenTexture", "assets/textures/whiteQueenTexture.png");
	this->loadTexture("whiteKingTexture", "assets/textures/whiteKingTexture.png");

	this->loadTexture("blackPawnTexture", "assets/textures/blackPawnTexture.png");
	this->loadTexture("blackRookTexture", "assets/textures/blackRookTexture.png");
	this->loadTexture("blackKnightTexture", "assets/textures/blackKnightTexture.png");
	this->loadTexture("blackBishopTexture", "assets/textures/blackBishopTexture.png");
	this->loadTexture("blackQueenTexture", "assets/textures/blackQueenTexture.png");
	this->loadTexture("blackKingTexture", "assets/textures/blackKingTexture.png");

	this->loadTexture("buttonTexture", "assets/textures/buttonTexture.png");
	this->loadTexture("buttonHoveredTexture", "assets/textures/buttonHoveredTexture.png");
	this->loadTexture("dataBoxTexture", "assets/textures/dataBoxTexture.png");

	this->loadTexture("whiteBoardTileTexture", "assets/textures/whiteBoardTileTexture.png");
	this->loadTexture("blackBoardTileTexture", "assets/textures/blackBoardTileTexture.png");
	this->loadTexture("selectedBoardTileTexture", "assets/textures/selectedBoardTileTexture.png");

	this->loadSound("buttonHoveredSound", "assets/sounds/buttonHoveredSound.mp3");
	this->loadSound("buttonPressedSound", "assets/sounds/buttonPressedSound.mp3");
	this->loadSound("pieceMoveSound", "assets/sounds/pieceMoveSound.mp3");
	this->loadSound("pieceCaptureSound", "assets/sounds/pieceCaptureSound.mp3");
	this->loadSound("boardStartSound", "assets/sounds/boardStartSound.mp3");
	this->loadSound("errorSound", "assets/sounds/errorSound.mp3");

	this->loadSound("win0Sound", "assets/sounds/win0Sound.mp3");
	this->loadSound("win1Sound", "assets/sounds/win1Sound.mp3");

	this->loadSound("lose0Sound", "assets/sounds/lose0Sound.mp3");

	this->loadSound("draw0Sound", "assets/sounds/draw0Sound.mp3");
	this->loadSound("draw1Sound", "assets/sounds/draw1Sound.mp3");
	this->loadSound("draw2Sound", "assets/sounds/draw2Sound.mp3");
	this->loadSound("draw3Sound", "assets/sounds/draw3Sound.mp3");

	this->loadFont("defaultFont", "assets/fonts/OpenSans-Regular.ttf");
}

GLuint AssetManager::getTexture(const std::string& textureName)
{
	if (this->textures.find(textureName) == this->textures.end())
	{
		std::cout << "Error: Texture " + textureName + " not found" << std::endl;
		return 0;
	}

	return this->textures[textureName].texture;
}

std::vector<AssetManager::Character>& AssetManager::getFont(const std::string& fontName)
{
	if (this->fonts.find(fontName) == this->fonts.end())
	{
		std::cout << "Error: Font " + fontName + " not found" << std::endl;
		return this->fonts.end()->second;
	}

	return this->fonts[fontName];
}

void AssetManager::playSound(const std::string& soundName, bool isLooping, bool canOverlapSameSound)
{
	if (this->sounds.find(soundName) == this->sounds.end())
	{
		std::cout << "Error: Sound " + soundName + " not found" << std::endl;
		return;
	}

	if (this->soundEnabled)
	{
		if (!canOverlapSameSound)
		{
			if (!this->soundEngine->isCurrentlyPlaying(this->sounds[soundName].c_str()))
			{
				this->soundEngine->play2D(this->sounds[soundName].c_str(), isLooping);
			}
		}
		else
		{
			this->soundEngine->play2D(this->sounds[soundName].c_str(), isLooping);
		}
	}
	else
	{
		std::cout << "Info: Sound " + soundName + " was not played because sound is disabled" << std::endl;
	}
}