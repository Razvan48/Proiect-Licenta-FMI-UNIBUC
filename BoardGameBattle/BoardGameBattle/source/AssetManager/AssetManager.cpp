#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

AssetManager& AssetManager::get()
{
	static AssetManager instance;
	return instance;
}

void AssetManager::loadResources()
{

}