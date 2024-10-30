#pragma once

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <irrKlang.h>

class AssetManager
{
public:
	struct Character
	{
		GLuint texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
	};

private:
	AssetManager();
	~AssetManager();
	AssetManager(const AssetManager& other) = delete;
	AssetManager& operator= (const AssetManager& other) = delete;
	AssetManager(const AssetManager&& other) = delete;
	AssetManager& operator= (const AssetManager&& other) = delete;

	struct Texture
	{
		GLuint texture;
		int width;
		int height;
		int nrChannels;
	};
	std::map<std::string, Texture> textures;
	std::map<std::string, std::string> sounds;

	void loadTexture(const std::string& textureName, const std::string& texturePath);
	void loadSound(const std::string& soundName, const std::string& soundPath);

	const int FONT_SIZE;
	void loadFont(const std::string& fontName, const std::string& fontPath);

	irrklang::ISoundEngine* soundEngine;

	FT_Library freeType;

	std::map<std::string, std::vector<AssetManager::Character>> fonts;

public:
	static AssetManager& get();
	void loadResources();
	GLuint getTexture(const std::string& textureName);
	std::vector<AssetManager::Character>& getFont(const std::string& fontName);
	void playSound(const std::string& soundName, bool isLooping);
};