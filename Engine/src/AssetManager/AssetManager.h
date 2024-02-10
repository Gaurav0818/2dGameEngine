#pragma once
#include <map>
#include <SDL_render.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& id, const std::string& filePath);
	void AddFont(const std::string& id, const std::string& filePath);
	void AddSound(const std::string& id, const std::string& filePath);

	SDL_Texture* GetTexture(const std::string& id) const;
	TTF_Font* GetFont(const std::string& id) const;
	Mix_Chunk* GetSound(const std::string& id) const;
	
private:
	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, TTF_Font*> m_fonts;
	std::map<std::string, Mix_Chunk*> m_sounds;
};
