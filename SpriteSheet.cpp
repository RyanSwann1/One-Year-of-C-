#include "SpriteSheet.h"
#include "SharedContext.h"
#include "TextureManager.h"
#include <sstream>
#include <fstream>
#include <iostream>

SpriteSheet::SpriteSheet(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext),
	m_currentAnimation(nullptr),
	m_tileSize(0)
{	
}

SpriteSheet::~SpriteSheet()
{
	for (auto &i : m_animations)
	{
		delete i.second.second;
		i.second.second = nullptr;
	}
	m_animations.clear();
}

void SpriteSheet::cropSprite()
{
	sf::Vector2i targetPos = m_currentAnimation->getDrawTargetPos();
	m_drawTargetPos.left = targetPos.x * m_tileSize;
	m_drawTargetPos.top = targetPos.y * m_tileSize;
	m_drawTargetPos.width = m_tileSize;
	m_drawTargetPos.height = m_tileSize;

	m_sprite.setTextureRect(m_drawTargetPos);
}

void SpriteSheet::setAnimationType(const AnimationName animationName, const Direction dir)
{
	//If current animation in use is differnet to newly requested animation
	if(m_currentAnimation)
	{
		if (m_currentAnimation->m_name != animationName || m_currentAnimation->m_direction != dir)
		{
			m_currentAnimation->stop();
		}
	}
	
	//Assign the new animation
	auto iter = m_animations.equal_range(animationName);
	for (auto i = iter.first; i != iter.second; ++i)
	{
		if (i->second.first == dir)
		{
			//Start the new animation
			m_currentAnimation = i->second.second;
			m_currentAnimation->play();
		}
	}
}

void SpriteSheet::update(const float deltaTime)
{
	if (m_currentAnimation)
	{
		m_currentAnimation->update(deltaTime);
		cropSprite();
	}
}

void SpriteSheet::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void SpriteSheet::loadIn(const std::string& fileName)
{
	std::fstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Cannot open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string type;
		keyStream >> type;
		if (type == "Texture")
		{
			keyStream >> m_spriteSheetName;
			if (m_sharedContext.m_textureManager->requireResource(m_spriteSheetName)) 
			{
				m_sprite.setTexture(*m_sharedContext.m_textureManager->getResource(m_spriteSheetName));
			}
			else {
				std::cout << "Couldn't open: " << m_spriteSheetName << "\n";
				return;
			}
		}
		else if (type == "Size")
		{
			keyStream >> m_tileSize;
		}
		else if (type == "Scale")
		{
			keyStream >> m_scale.x >> m_scale.y;
			m_sprite.setScale(m_scale);
		}
		else if (type == "SheetSize")
		{
			keyStream >> m_size.x >> m_size.y;
		}
		else if (type == "Animation")
		{
			//StartFrame | EndFrame | Row | FrameTime |
			//std::string animationName;
			int animationName = 0;
			int startFrame = 0;
			int endFrame = 0;
			int row = 0;
			float frameTime = 0;
			int direction = 0;
			int repeatable = 0;
			keyStream >> animationName >> startFrame >> endFrame >> row >> frameTime >> direction >> repeatable;

			Animation* animation = new Animation(animationName, startFrame, endFrame, row, frameTime, direction, repeatable);
			m_animations.emplace(std::make_pair(static_cast<AnimationName>(animationName), std::make_pair(static_cast<Direction>(direction), animation)));
		}
	}
	file.close();
}