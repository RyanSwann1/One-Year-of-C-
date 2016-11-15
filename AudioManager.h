#pragma once

#include "ResourceManager.h"
#include <SFML\Audio.hpp>
#include <fstream>
#include <sstream>
#include <iostream>



class AudioManager 
	: public ResourceManager<sf::SoundBuffer>
{
public:
	AudioManager(const std::string& fileName)
	{
		loadIn();
	}

	~AudioManager() 
	{
		purgeResources();
	}



private:
	sf::SoundBuffer* loadFromFile(const std::string& fileName)
	{
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
		if (!soundBuffer->loadFromFile(fileName))
		{
			delete soundBuffer;
			soundBuffer = nullptr;
			return nullptr;
		}
		return soundBuffer;
	}

	void loadIn()
	{
		registerFilePath("PlayerJump", "PlayerJump.Wav");
		registerFilePath("PlayerAttack", "PlayerAttack.Wav");
		registerFilePath("PlayerDeath", "PlayerDeath.Wav");
		registerFilePath("RatDeath", "RatDeath.Wav");
		registerFilePath("PlayerHurt", "PlayerHurt.Wav");
		registerFilePath("GameBackgroundMusic", "GameBackgroundMusic.FLAC");
	}
};