#include "AssetManager.h"

#include <filesystem>
#include <SDL_image.h>

#include "../Logger/Logger.h"
AssetManager::AssetManager()
{
	Logger::Info("AssetManager Constructor called!");	
}

AssetManager::~AssetManager()
{
	Logger::Info("AssetManager Destructor called!");
	ClearAssets();
}

void AssetManager::ClearAssets()
{
	for(auto texture : m_textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	m_textures.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string& id, const std::string& filePath)
{
	if(!std::filesystem::exists(filePath))
	{
		Logger::Error("File does not exist: " + filePath );
		return;
	}
	
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(texture == nullptr)
	{
		Logger::Error("Error loading texture: " + filePath);
		return;
	}


	// Add the texture to the map
	m_textures.emplace(id, texture);
	Logger::Info("Texture added to AssetManager : " + id);
}

void AssetManager::AddFont(const std::string& id, const std::string& filePath)
{
	if(!std::filesystem::exists(filePath))
	{
		Logger::Error("File does not exist: " + filePath );
		return;
	} 
}

void AssetManager::AddSound(const std::string& id, const std::string& filePath)
{
	if(!std::filesystem::exists(filePath))
	{
		Logger::Error("File does not exist: " + filePath );
		return;
	} 
}

SDL_Texture* AssetManager::GetTexture(const std::string& id) const
{
	return m_textures.at(id);
}

TTF_Font* AssetManager::GetFont(const std::string& id) const
{
	return nullptr;
}

Mix_Chunk* AssetManager::GetSound(const std::string& id) const
{
	return nullptr;
}