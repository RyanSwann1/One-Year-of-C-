#pragma once

#include "Direction.h"
#include "EntityType.h"
#include "Collision.h"
#include "SharedContext.h"
#include <SFML\Graphics.hpp>
#include <vector>

class Entity;
struct Tile;

struct CollisionBox
{
	CollisionBox(const sf::Vector2f& pos)
	{
		m_shape.setPosition(pos);
		m_shape.setSize(sf::Vector2f(32, 32));
		m_shape.setFillColor(sf::Color::Green);
	}

	sf::RectangleShape m_shape;
};

struct CollisionElement
{
	CollisionElement(const float area, const sf::FloatRect intersection, const sf::FloatRect collisionBox)
		: m_area(area),
		m_intersection(intersection),
		m_collisionBox(collisionBox)
	{}

	float m_area;
	sf::FloatRect m_intersection;
	sf::FloatRect m_collisionBox;
};

struct SharedContext;
class CollisionManager
{
public:
	CollisionManager(const SharedContext& sharedContext, Entity& entity);
	~CollisionManager();

	bool collidingOnXAxis() const { return m_collidingOnX; }
	bool collidingOnYAxis() const { return m_collidingOnY; }
	inline void resetCollisionChecks()
	{
		m_collidingOnX = false;
		m_collidingOnY = false;
	}
	
	void draw(sf::RenderWindow& window);
	void update();
	Tile* getReferenceTile() const { return m_referenceTile; }

private:
	SharedContext m_sharedContext;
	Tile* m_referenceTile; //Tile that entity is currently standing on
	Entity* m_entity;
	Collision m_collision;
	std::vector<CollisionElement> m_collisions;
	sf::RectangleShape m_playerBox;
	std::vector<CollisionBox> m_collisionBoxes;
	bool m_collidingOnY;
	bool m_collidingOnX;
	
	void checkEntityCollisions();
	void checkTileCollisions();
	void resolveCollisions();
};