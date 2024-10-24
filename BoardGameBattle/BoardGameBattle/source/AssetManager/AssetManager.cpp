#include "AssetManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <irrKlang.h>

#include <iostream>

AssetManager::AssetManager()
{
	//stbi_set_flip_vertically_on_load(true);
}

AssetManager::~AssetManager()
{

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

	glEnable(GL_TEXTURE_2D); // TODO: nu cred ca e necesar
	glGenTextures(1, &this->textures[textureName].texture);
	glBindTexture(GL_TEXTURE_2D, this->textures[textureName].texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->textures[textureName] = AssetManager::Texture();
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

	std::cout << "Loaded texture " + textureName + " at path : " << texturePath << std::endl;
	std::cout << "Size: " << this->textures[textureName].width << "x" << this->textures[textureName].height << std::endl;
	std::cout << "Number of channels: " << this->textures[textureName].nrChannels << std::endl;
}

void AssetManager::loadSound(const std::string& soundName, const std::string& soundPath)
{
	if (this->sounds.find(soundName) != this->sounds.end())
		std::cout << "Warning: Sound " + soundName + " already loaded" << std::endl;
	else
		this->sounds[soundName] = soundPath;
}

void AssetManager::loadResources()
{
	this->loadTexture("test", "assets/textures/test.png");
	//this->loadTexture("test2", "assets/textures/test2.jpg");

	this->loadSound("test", "assets/sounds/test.mp3");
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

void AssetManager::playSound(const std::string& soundName, bool loop)
{
	if (this->sounds.find(soundName) == this->sounds.end())
	{
		std::cout << "Error: Sound " + soundName + " not found" << std::endl;
		return;
	}

	static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
	soundEngine->play2D(this->sounds[soundName].c_str(), loop);
}