#pragma once

#include "ResourceManager.h"
#include <SFML\Graphics.hpp>
#include <iostream>

class FontManager 
	: public ResourceManager<sf::Font>
{
public:
	FontManager()
	{
		loadIn();
	}

	~FontManager()
	{
		purgeResources();
	}

	sf::Font* loadFromFile(const std::string& fileName) override
	{
		sf::Font* font = new sf::Font;
		if (!font->loadFromFile(fileName))
		{
			delete font;
			font = nullptr;
			return nullptr;
		}
		return font;
	}

private:
	void loadIn()
	{
		registerFilePath("Magnificent", "Magnificent.ttf");
	}
};