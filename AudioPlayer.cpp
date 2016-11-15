#include "AudioPlayer.h"
#include "AudioManager.h"
#include "SharedContext.h"
#include <iostream>
#include <fstream>
#include <sstream>


AudioPlayer::AudioPlayer(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext),
	m_currentAudioClips(nullptr)
{
}

AudioPlayer::~AudioPlayer()
{
	AudioManager* audioManager = m_sharedContext.m_audioManager;
	for (auto &i : m_owners)
	{
		for (auto &audioClip : i.second)
		{
			audioManager->releaseResource(audioClip.m_name);
		}
	}
	m_owners.clear();
}

void AudioPlayer::registerOwner(const std::string & ownerID)
{
	m_currentOwner = ownerID;
	auto cIter = m_owners.find(ownerID);
	if (cIter != m_owners.cend())
	{
		m_currentAudioClips = &cIter->second;
	}
	else 
	{
		std::cerr << "\nOwner not found.\n";
	}
}

void AudioPlayer::registerAudioClip(const std::string& audioOwner, const std::string & audioClipName)
{
	AudioManager& audioManager = *m_sharedContext.m_audioManager;
	sf::Sound* audioClip = new sf::Sound;
	auto owner = m_owners.find(audioOwner);
	if (owner != m_owners.cend())
	{	
		if (audioManager.requireResource(audioOwner + audioClipName))
		{
			audioClip->setBuffer(*audioManager.getResource(audioOwner + audioClipName));
			owner->second.emplace_back(AudioClip(audioClipName, audioClip));
		}
	}
	else
	{
		if (audioManager.requireResource(audioOwner + audioClipName))
		{
			audioClip->setBuffer(*audioManager.getResource(audioOwner + audioClipName));
			m_owners.emplace(audioOwner, std::vector<AudioClip>{AudioClip(audioClipName, audioClip)});
		}
	}
}

void AudioPlayer::removeAudioClip(const std::string & audioOwner, const std::string & audioClipName)
{
	AudioManager* audioManager = m_sharedContext.m_audioManager;
	for (auto &i : m_owners)
	{
		for (auto iter = i.second.begin(); iter != i.second.end(); ++iter)
		{
			if (iter->m_name == audioClipName)
			{
				audioManager->releaseResource(audioOwner + audioClipName);
				i.second.erase(iter);
				return;
			}
		}
	}
}

void AudioPlayer::removeOwner(const std::string & audioOwner)
{
	AudioManager& audioManager = *m_sharedContext.m_audioManager;
	for (auto iter = m_owners.begin(); iter != m_owners.end(); ++iter)
	{
		if (iter->first == audioOwner)
		{
			for (auto &i : iter->second)
			{
				audioManager.releaseResource(audioOwner + i.m_name);
				i.m_audioClip = nullptr;
			}
			m_owners.erase(iter);
			return;
		}
	}
}

void AudioPlayer::play(const std::string & audioClipName, const bool& loop)
{
	sf::Sound* audioClip = findAudioClip(audioClipName);
	if (audioClip)
	{
		//audioClip->play();
		//audioClip->setLoop(loop);
	}
}

void AudioPlayer::stop(const std::string & audioClipName)
{
	sf::Sound* audioClip = findAudioClip(audioClipName);
	if (audioClip)
	{
		audioClip->stop();
	}
}

sf::Sound* AudioPlayer::findAudioClip(const std::string & audioClipName)
{
	if (m_currentAudioClips)
	{
		for (auto iter = m_currentAudioClips->begin(); iter != m_currentAudioClips->end(); ++iter)
		{
			if (iter->m_name == audioClipName)
			{
				return iter->m_audioClip;
			}
		}
	}

	auto owner = m_owners.find(m_currentOwner);
	if (owner != m_owners.cend())
	{
		auto audioClips = owner->second;
		for (auto audioClip = audioClips.begin(); audioClip != audioClips.end(); ++audioClip)
		{
			if (audioClip->m_name== audioClipName)
			{
				return audioClip->m_audioClip;
			}
		}
	}

	//not found
	return nullptr;
}

void AudioPlayer::loadInAudioClips(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Couldn't open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		std::string owner;
		std::string audioClipName;

	}
	file.close();
}