#pragma once

#include "EntityType.h"
#include "Direction.h"
#include "SpriteSheet.h"
#include "AudioPlayer.h"
#include "CollisionManager.h"
#include "SharedContext.h"
#include <unordered_map>
#include <vector>
#include <SFML\Graphics.hpp>
#include <string>



//
struct Tile;
struct TileInfo;
struct SharedContext;

class Entity
{
	friend class EntityManager;
	friend class CollisionManager;
public:
	Entity(const SharedContext& sharedContext);
	virtual ~Entity();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	
	inline void setVelocity(const float x, const float y) { m_velocity = sf::Vector2f(x, y); }

	void setAcceleration(const float x, const float y) { m_acceleration = sf::Vector2f(x, y); }
	void setPosition(const sf::Vector2f& pos) { m_position = pos; }
	inline void setID(const int ID)
	{
		if (ID >= 0)
		{
			m_ID = ID;
		}
	}
	inline void setInitialPosition(const sf::Vector2f& initialPos)
	{
		m_position = initialPos;
		m_startPosition = initialPos;
	}

	const EntityType getType() { return m_type; }
	sf::Vector2i getPosition() const;
	const sf::Vector2f& getActualPosition() { return m_position; } //Hacky, needs real name / Maybe change of how i get positions
	const std::string& getName() { return m_name; }
	const int getID() { return m_ID; }
	const sf::FloatRect getAABB() { return m_AABB; }
	Direction getDirection() const { return m_currentDirection; }
	const Tile* getReferenceTile() { return m_collisionManager.getReferenceTile(); }

	void addVelocity(const float x, const float y);
	void move(const float x, const float y);
	void accelerate(const float x, const float y) { m_acceleration += sf::Vector2f(x, y); }
	void applyFriction(const float deltaTime);
	void applyGravity();
	void updateAABB();
	void stop() { m_velocity.x = 0; }

	virtual void update(const float deltaTime);
	virtual void onEntityCollision(Entity& entity) = 0;
	virtual void draw(sf::RenderWindow& window);

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_startPosition;
	sf::Vector2f m_oldPosition;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_velocity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_friction;
	sf::Vector2f m_minFrictionValue;
	sf::Vector2f m_gravity;
	sf::FloatRect m_AABB;
	std::string m_name;

	Direction m_currentDirection;
	EntityType m_type;
	SpriteSheet m_spriteSheet;
	SharedContext m_sharedContext;
	AudioPlayer m_audioPlayer;
	CollisionManager m_collisionManager;

	virtual void resolveCollisions() = 0;
	virtual void load(const std::string& fileName) {}

private:
	int m_ID;
};

