#pragma once

#include <SFML\Graphics.hpp>

#include "ResourceManager.h"
class TextureManager
	: public ResourceManager<sf::Texture>
{
public:
	TextureManager(const std::string& fileName)
	{
		loadInFilePath();
	}

	~TextureManager()
	{
		purgeResources();
	}

private:
	sf::Texture* loadFromFile(const std::string& id) override
	{
		sf::Texture* texture = new sf::Texture;
		if (!texture->loadFromFile(id))
		{
			delete texture;
			texture = nullptr;
			return nullptr;
		}
		return texture;
	}

	void loadInFilePath()
	{
		registerFilePath("TileSheet", "TileSheet.PNG");
		registerFilePath("PlayerSheet", "PlayerSheet.PNG");
		registerFilePath("RatSheet", "RatSheet.PNG");
		registerFilePath("Background", "Background.PNG");
	}
};