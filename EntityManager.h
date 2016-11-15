#pragma once

#include "Entity.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <list>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void add(Entity* entity);
	void add(Entity* entity, const sf::Vector2f& pos);
	void addExisting(const EntityType entityType, const sf::Vector2i& pos);
	void removeEntity(const unsigned int id);

	void purgeEntities();
	Entity* findEntity(const std::string& name);
	Entity* getEntityAtPosition(const sf::Vector2i& pos);
	std::string getEntityTypeLocation(const std::string& name) const;

	void update(const float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<EntityType, std::function<Entity*()>> m_entityFactory;
	std::unordered_map<std::string, std::string> m_entityTypes;
	std::list<std::pair<unsigned int, Entity*>> m_entities;
	std::vector<unsigned int> m_removals;

	unsigned int m_entityID;
	void processRemovals();
	void removeActiveEntity(unsigned int entityID);
	void loadInEntityTypes(const std::string& fileName);

	void purgeFactory();
};

