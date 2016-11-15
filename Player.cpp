#include "Player.h"
#include "EventManager.h"
#include "SharedContext.h"
#include "EntityManager.h"
#include "AudioManager.h"
#include "StateManager.h"
#include "Map.h"
#include <iostream>

Player::Player(SharedContext& sharedContext)
	: Character(sharedContext)
{
	m_type = EntityType::Player;
	m_name = "Player";
	Character::load(m_sharedContext.m_entityManager->getEntityTypeLocation(m_name));
	setDirection(Direction::Right);

	m_audioPlayer.registerAudioClip(m_name, "Attack");
	m_audioPlayer.registerAudioClip(m_name, "Jump");
	m_audioPlayer.registerAudioClip(m_name, "Death");
	m_audioPlayer.registerAudioClip(m_name, "Hurt");
	m_audioPlayer.registerOwner(m_name);

	EventManager& eventManager = *m_sharedContext.m_eventManager;
	eventManager.addCallback<Player>("Move_Left", StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>("Move_Right", StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>("Jump", StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>("Stop", StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>("Action_Button", StateType::Game, &Player::reactToInput, this);
}

Player::~Player()
{
	EventManager& eventManager = *m_sharedContext.m_eventManager;
	eventManager.removeCallback("Move_Left");
	eventManager.removeCallback("Move_Right");
	eventManager.removeCallback("Jump");
	eventManager.removeCallback("Action_Button");

	m_audioPlayer.removeAudioClip(m_name, "Attack");
	m_audioPlayer.removeAudioClip(m_name, "Jump");
	m_audioPlayer.removeAudioClip(m_name, "Death");
	m_audioPlayer.removeAudioClip(m_name, "Hurt");
	m_audioPlayer.removeOwner(m_name);
}

void Player::onEntityCollision(Entity& entity)
{
	if (entity.getType() == EntityType::Enemy)
	{
		Character* character = static_cast<Character*>(&entity);
		reduceLife(character->getDamageValue());
	}
}

void Player::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_name == "Move_Left")
	{
		move(Direction::Left);
	}
	else if (eventDetails.m_name == "Move_Right")
	{
		move(Direction::Right);
	}
	else if (eventDetails.m_name == "Action_Button")
	{
		attackPreperation();
	}
	else if (eventDetails.m_name == "Jump")
	{
		jump();
	}
	else if (eventDetails.m_name == "Stop")
	{
		m_velocity.x = 0;
	}
}

void Player::resolveCollisions()
{
	Character::resolveCollisions();

	Tile* referenceTile = m_collisionManager.getReferenceTile();
	if (referenceTile)
	{
		//Standing on dedadly tile
		if (referenceTile->m_deadly) 
		{
			killCharacter();
		}
		//Standing on tile that'll change level
		else if (referenceTile->m_warp) 
		{
			Map& map = *m_sharedContext.m_map;
			map.loadNextLevel();
		}
	}
}