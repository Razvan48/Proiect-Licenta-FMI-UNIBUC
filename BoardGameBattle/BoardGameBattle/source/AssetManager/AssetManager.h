#pragma once

#include <string>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class AssetManager
{
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

public:
	static AssetManager& get();
	void loadResources();
	GLuint getTexture(const std::string& textureName);
	void playSound(const std::string& soundName, bool loop);
};