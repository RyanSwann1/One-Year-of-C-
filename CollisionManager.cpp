#include "CollisionManager.h"
#include "Map.h"
#include "Entity.h"
#include "EntityManager.h"
#include <math.h>
#include <iostream>

CollisionManager::CollisionManager(const SharedContext& sharedContext, Entity& entity)
	: m_sharedContext(sharedContext),
	m_entity(&entity),
	m_referenceTile(nullptr),
	m_collidingOnX(false),
	m_collidingOnY(false)
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::draw(sf::RenderWindow & window)
{
	//window.draw(m_playerBox);

	for (const auto &i : m_collisionBoxes)
	{
		window.draw(i.m_shape);
	}
}

void CollisionManager::update()
{
	checkTileCollisions();
	resolveCollisions();
	checkEntityCollisions();

	sf::FloatRect AABB = m_entity->getAABB();
	m_playerBox.setPosition((AABB.left + AABB.width), AABB.top);
	m_playerBox.setSize(sf::Vector2f(32, 32));
	m_playerBox.setFillColor(sf::Color::Red);

	m_collisions.clear();

	if (!m_collidingOnY)
	{
		m_referenceTile = nullptr;
	}
}

void CollisionManager::checkEntityCollisions()
{
	EntityManager& entityManager = *m_sharedContext.m_entityManager;
	Entity* entity2 = entityManager.getEntityAtPosition(m_entity->getPosition());
	if (entity2)
	{
		m_entity->onEntityCollision(*entity2);
		entity2->onEntityCollision(*m_entity);
	}
}

void CollisionManager::checkTileCollisions()
{

	int tileSize = Sheet::TILE_SIZE;
	Map& map = *m_sharedContext.m_map;
	sf::FloatRect AABB = m_entity->getAABB();

	int fromX = std::floor((AABB.left - (AABB.width)) / tileSize);
	int toX = std::floor((AABB.left + (AABB.width * 2)) / tileSize);
	int fromY = std::floor((AABB.top - (AABB.height)) / tileSize);
	int toY = std::floor((AABB.top + (AABB.height * 2)) / tileSize);

	for (int y = fromY; y < toY; ++y)
	{
		for (int x = fromX; x < toX; ++x)
		{
			Tile* tile = map.getTile(x, y);
			if (tile)
			{
				sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);

				
				sf::FloatRect intersection;
				if (AABB.intersects(tileBounds, intersection))
				{
					//m_collisionBoxes.push_back(CollisionBox(sf::Vector2f(tileBounds.left, tileBounds.top)));
					float area = intersection.height * intersection.width;
					m_collisions.push_back(CollisionElement(area, intersection, tileBounds));
				}
			}
		}
	}


}

void CollisionManager::resolveCollisions()
{
	if (m_collisions.empty()) {
		return;
	}

	//Sort so collisions are in greatest area order
	std::sort(m_collisions.begin(), m_collisions.end(), [](const CollisionElement& col1, const CollisionElement& col2) {return col1.m_area > col2.m_area; });

	for (const auto &i : m_collisions)
	{
		sf::FloatRect AABB = m_entity->getAABB();
		//Make sure that entity is still colliding with the collision box in question
		if (!AABB.intersects(i.m_collisionBox))
		{
			continue;
		}
		
		sf::FloatRect collisionBox = i.m_collisionBox;
		float xDifference = (AABB.left + (AABB.width / 2.0f)) - (collisionBox.left + (collisionBox.width / 2.0f));
		float yDifference = (AABB.top + (AABB.height / 2.0f)) - (collisionBox.top + (collisionBox.height / 2.0f));
		float resolve = 0;

		sf::FloatRect intersection = i.m_intersection;
		if (std::abs(xDifference) > std::abs(yDifference))
		{
			if (xDifference > 0) {
				resolve = intersection.width;
			}
			else {
				resolve = -intersection.width;
			}
			m_entity->move(resolve, 0);
			m_entity->m_velocity.x = 0;
			m_collidingOnX = true;
		}
		else
		{
			if (yDifference > 0) {
				resolve = intersection.height;
			}
			else {
				resolve = -intersection.height;
			}
			m_entity->move(0, resolve);
			m_entity->m_velocity.y = 0;
			m_collidingOnY = true;
		}
		//Assign the reference tile for the entity
		if (m_collidingOnY)
		{
			//m_collisionBoxes.push_back(CollisionBox(sf::Vector2f(collisionBox.left, collisionBox.top)));
			Map& map = *m_sharedContext.m_map;
			Tile* tile = map.getTile(std::floor(collisionBox.left / Sheet::TILE_SIZE), std::floor(collisionBox.top / Sheet::TILE_SIZE));
			if (tile)
			{
				m_referenceTile = tile;
			}
		}
	}
	m_collisions.clear();
}

