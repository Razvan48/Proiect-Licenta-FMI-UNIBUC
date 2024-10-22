#pragma once

#include <string>
#include <map>

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

	};

	std::map<std::string, Texture> textures;

public:
	static AssetManager& get();
	void loadResources();
};