#include "Rat.h"
#include "SharedContext.h"
#include "EntityManager.h"
#include "Utilities.h"

Rat::Rat(const SharedContext& sharedContext)
	: Computer(sharedContext),
	m_attackReady(true)
{
	
	m_type = EntityType::Enemy;
	m_currentDirection = static_cast<Direction>(Utilities::randomNumberGenerator(0, 1));
	m_name = "Rat";
	m_audioPlayer.registerAudioClip(m_name, "Death");
	m_audioPlayer.registerOwner(m_name);
	Character::load(m_sharedContext.m_entityManager->getEntityTypeLocation(m_name));
	m_attackTimer.setExpirationTime(4);
	m_attackTimer.activate();
	m_position = sf::Vector2f(400, 400);
}

Rat::~Rat()
{
}

void Rat::update(const float deltaTime)
{
	Computer::update(deltaTime);

	handleMovement();
	handleAttack(deltaTime);

	m_attackTimer.update(deltaTime);
	if (m_attackTimer.isExpired())
	{
		m_attackTimer.reset();
		m_attackReady = true;
	}
}

void Rat::onEntityCollision(Entity& entity)
{
	if (m_currentState != EntityState::Dead)
	{
		if (entity.getType() == EntityType::Player)
		{
			Character* character = static_cast<Character*>(&entity);
			if (character->getState() == EntityState::Attacking)
			{
				reduceLife(character->getDamageValue());
			}
		}
	}
}

void Rat::handleAttack(const float deltaTime)
{
	if (m_attackReady)
	{
		attackPreperation();
		m_attackReady = false;
	}
}

void Rat::handleMovement()
{
	switch (m_currentDirection)
	{
	case (Direction::Right) :
	{
		move(Direction::Right);
		break;
	}
	case (Direction::Left) : 
	{
		move(Direction::Left);
		break;
	}
	}
}