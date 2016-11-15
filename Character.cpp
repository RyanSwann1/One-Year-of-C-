#include "Character.h"
#include "SharedContext.h"
#include "Map.h"
#include "EntityManager.h"
#include "AnimationName.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

Character::Character(const SharedContext& sharedContext)
	: Entity(sharedContext),
	m_jumpReady(true),
	m_jumpTime(0.75f),
	m_hurtTime(1.0f),
	m_hurt(false)
{
	m_jumpingTimer.setExpirationTime(m_jumpTime);
	m_hurtTimer.setExpirationTime(m_hurtTime);
}

Character::~Character()
{
}

void Character::load(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string type;
		std::string spriteSheetName;

		keyStream >> type;
		if (type == "SpriteSheetDetails")
		{
			keyStream >> spriteSheetName;
			m_spriteSheet.loadIn(spriteSheetName);

			int tileSize = m_spriteSheet.getTileSize();
			m_attackAABB.width = tileSize;
			m_attackAABB.height = tileSize;
		}
		else if (type == "Lives")
		{
			keyStream >> m_lives >> m_maxLives;
		}
		else if (type == "Damage")
		{
			keyStream >> m_damage;
		}
		else if (type == "Speed")
		{
			keyStream >> m_speed.x >> m_speed.y;
		}
		else if (type == "JumpVelocity")
		{
			keyStream >> m_jumpVelocity;
		}
		else if (type == "MaxVelocity")
		{
			keyStream >> m_maxVelocity.x >> m_maxVelocity.y;
		}
		else if (type == "Position")
		{
			keyStream >> m_position.x >> m_position.y;
			m_startPosition = m_position;
		}
		else if (type == "JumpTime")
		{
			keyStream >> m_jumpTime;
		}
	}
}

void Character::move(const Direction dir)
{
	if (m_currentState != EntityState::Dead && m_currentState != EntityState::Attacking && m_currentState != EntityState::Hurt)
	{
		switch (dir)
		{
		case (Direction::Left):
		{
			setDirection(Direction::Left);
			setVelocity(-m_speed.x, m_velocity.y);
			break;
		}
		case (Direction::Right):
		{
			setDirection(Direction::Right);
			setVelocity(m_speed.x, m_velocity.y);
			break;
		}
		}
	}
}

void Character::update(const float deltaTime)
{
	Entity::update(deltaTime);

	if (m_currentState != EntityState::Attacking &&
		m_currentState != EntityState::Hurt &&
		m_currentState != EntityState::Dead)
	{
		if (m_velocity.x < -1.0f || m_velocity.x > 1.0f)
		{
			setState(EntityState::Walking);

		}
		else {
			setState(EntityState::Idle);
		}
	}

	handleTimers(deltaTime);
	handleAnimations();
	m_spriteSheet.setSpritePosition(m_position);
	m_spriteSheet.update(deltaTime);
}

void Character::draw(sf::RenderWindow & window)
{
	Entity::draw(window);

	window.draw(m_attackShape);
}

void Character::reduceLife(const int damage)
{
	//Only allow character to take damage periodic intervals
	if (m_currentState == EntityState::Dead || m_hurtTimer.isActive()) {
		return;
	}

	if ((m_lives - damage) <= 0)
	{
		killCharacter();
		m_lives = 0;
		//m_velocity.x = 0;
		//setState(EntityState::Dead);
		//m_spriteSheet.setAnimationType("Dead", m_currentDirection);	
	}
	else 
	{
		m_spriteSheet.setAnimationType(AnimationName::Hurt, m_currentDirection);
		m_audioPlayer.play("Hurt", false);
		setState(EntityState::Hurt);
		m_lives -= damage;
		m_hurtTimer.activate(); 
		m_velocity.x = 0;
	}
}

void Character::jump()
{
	if (m_jumpReady && m_currentState != EntityState::Dead)
	{
		m_audioPlayer.play("Jump", false);
		m_jumpReady = false;
		m_jumpingTimer.activate();
	}
}

void Character::killCharacter()
{
	if (m_currentState != EntityState::Dead)
	{
		m_audioPlayer.play("Hurt", false);
		setState(EntityState::Dead);
		m_spriteSheet.setAnimationType(AnimationName::Dead, m_currentDirection);
		m_velocity = sf::Vector2f(0, 0);
	}
}

void Character::attackPreperation()
{
	if (m_currentState == EntityState::Dead ||
		m_currentState == EntityState::Attacking ||
		m_currentState == EntityState::Hurt) {
		return;
	}

	setState(EntityState::Attacking);
	m_spriteSheet.setAnimationType(AnimationName::Attack, m_currentDirection);
	m_audioPlayer.play("Attack", false);
	m_velocity.x = 0;
	int tileSize = m_spriteSheet.getTileSize();
	m_attackAABB.top = m_AABB.top;
	int y = m_attackAABB.top / tileSize;
	int fromX = 0;
	int toX = 0;
	Map& map = *m_sharedContext.m_map;

	//Get attack information from specific direction
	switch (m_currentDirection)
	{
	case (Direction::Right):
	{
		m_attackAABB.left = m_AABB.left;
		
		fromX = std::floor((m_attackAABB.left) / tileSize);
		toX = std::floor((m_attackAABB.left + (m_attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	case (Direction::Left):
	{
		m_attackAABB.left = m_AABB.left;

		toX = std::floor(m_attackAABB.left / tileSize);
		fromX = std::floor((m_attackAABB.left - (m_attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	}

	/*m_attackShape.setPosition(m_attackAABB.left, m_attackAABB.top);
	m_attackShape.setSize(sf::Vector2f(32, 32));
	m_attackShape.setFillColor(sf::Color::Red);*/
	std::vector<std::pair<sf::Vector2i, Entity*>> entitiesAtPos;
	for (int x = fromX; x < toX; ++x)
	{
		Entity* entity = m_sharedContext.m_entityManager->getEntityAtPosition(sf::Vector2i(x, y));
		if (entity)
		{
			entitiesAtPos.emplace_back(std::make_pair(sf::Vector2i(x, y), entity));
		}
	}

	for (const auto &i : entitiesAtPos)
	{
		//Do not allow to attack the same Entity
		if (i.second->getID() != getID())
		{
			attack(*i.second, i.first.x, i.first.y);
		}
	}
}

void Character::attack(Entity& entity, const int x, const int y)
{
	int tileSize = m_spriteSheet.getTileSize();
	sf::Vector2i entityPos = entity.getPosition();
	sf::FloatRect attackBox(x * tileSize, y * tileSize, tileSize, tileSize);
	if (attackBox.intersects(entity.getAABB()))
	{
		entity.onEntityCollision(*this);
	}
}

void Character::handleAnimations()
{
	switch (m_currentState)
	{
	case (EntityState::Idle) :
	{
		//Make the spritesheet/Animation class handle the difference between the direction
		//m_spriteSheet.setAnimationType("Idle", m_currentDirection);
		//m_spriteSheet.setAnimationType(AnimationNames::Names::m_animationNames[static_cast<int>(EntityState::Idle)], m_currentDirection);
		m_spriteSheet.setAnimationType(AnimationName::Idle, m_currentDirection);
		break;
	}
	case (EntityState::Walking) :
	{
		m_spriteSheet.setAnimationType(AnimationName::Walk, m_currentDirection);
		break;
	}
	case (EntityState::Attacking) :
	{
		//Attack animation will not repeat
		if (m_spriteSheet.getCurrentAnimation()->isFinished())
		{
			m_spriteSheet.getCurrentAnimation()->stop();
			if (m_velocity.x <= -0.1f || m_velocity.x >= 0.1f)
			{
				setState(EntityState::Walking);
			}
			else {
				setState(EntityState::Idle);
			}
		}
		break;
	}
	case (EntityState::Dead) :
	{
		if (m_spriteSheet.getCurrentAnimation()->isFinished())
		{
			m_sharedContext.m_entityManager->removeEntity(getID());
		}
		break;
	}
	case (EntityState::Hurt) :
	{
		if (m_spriteSheet.getCurrentAnimation()->isFinished())
		{
			setState(EntityState::Idle);
		}
	}
	}
}

void Character::handleTimers(const float deltaTime)
{
	if (m_hurtTimer.isActive())
	{
		m_hurtTimer.update(deltaTime);
		if (m_hurtTimer.isExpired())
		{
			m_hurtTimer.deactivate();
		}
	}

	if (m_jumpingTimer.isActive())
	{
		m_jumpingTimer.update(deltaTime);
		addVelocity(0, -m_jumpVelocity);

		if (m_jumpingTimer.isExpired())
		{
			m_jumpingTimer.deactivate();
		}
	}
}

void Character::resolveCollisions()
{
	if (m_collisionManager.collidingOnYAxis()) {
		m_jumpReady = true;
	}
}