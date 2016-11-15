#include "Map.h"
#include "Utilities.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "Rat.h"
#include "Entity.h"
#include "Window.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include <iostream>

Map::Map(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext),
	m_maxMapSize(30, 30),
	m_defaultFriction(0.02f, 0.1f),
	m_gravity(0.1f, 0.1f),
	m_tileSize(0),
	m_nextLevel(false),
	m_currentMapIndex(0)
{
	m_sharedContext.m_map = this;
	loadInMaps("Maps.txt");
	loadInTiles("Tiles.txt");
}

Map::~Map()
{
	m_tileMap.clear();
	TextureManager& textureManager = *m_sharedContext.m_textureManager;
	textureManager.releaseResource(m_backgroundName);
	purgeTileMap();
	purgeTileSet();
}

void Map::loadMap(const std::string & mapName)
{
	std::ifstream file;
	file.open(mapName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open: " << mapName << "\n";
		return;
	}
	//Acts as coordinates
	unsigned int x = 0;
	unsigned int y = 0;
	std::string line;
	//Cycle through the line
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		//unsigned int id = 0;
		for (const auto &i : line)
		{
			unsigned int id = 0;
			if (std::atoi(&i) > 0)
			{
				 id = std::atoi(&i) - 1;
				 //std::cout << "Tile ID: " << id << "\n";
				 //Find appropriate tile
				 auto iter = m_tileSet.find(id);
				 if (iter != m_tileSet.cend())
				 {
					 Tile* tile = new Tile;
					 
					 tile->m_tileInfo = new TileInfo(*iter->second);
					 tile->m_tileInfo->m_position = sf::Vector2i(x, y);
					 //HACK - Meaning that tile is deadly
					 //Will have to use XML at some point to correctly do this all
					 if (tile->m_tileInfo->m_ID == 7) 
					 {
						 tile->m_deadly = true;
					 }
					 else if (tile->m_tileInfo->m_ID == 8)
					 {
						 tile->m_warp = true;
					 }
					 m_tileMap.emplace_back(std::make_pair(convertCoords(x, y), tile));
					 //if (!m_tileMap.emplace_back(std::make_pair(convertCoords(x, y), tile)).second)
					 //{
						// std::cerr << "Duplicate tile in position: " << x << " " << y << "\n";
					 //}
				 }
			}
			++x;
		}
		++y;
		x = 0;

		std::string type;
		keyStream >> type;
		if (type == "Background")
		{
			std::string textureName;
			keyStream >> textureName;
			m_backgroundName = textureName;

			TextureManager* textureManager = m_sharedContext.m_textureManager;
			if (textureManager->requireResource(m_backgroundName))
			{
				m_backgroundImage.setTexture(*textureManager->getResource(m_backgroundName));
			}

			m_backgroundImage.setScale(4, 4.2);
			m_backgroundImage.setPosition(250, 0);

		}
		else if (type == "Entity")
		{
			std::string entityName;
			keyStream >> entityName;
			sf::Vector2f spawnPosition;
			keyStream >> spawnPosition.x >> spawnPosition.y;

			if (entityName == "Rat")
			{
				m_sharedContext.m_entityManager->add(new Rat(m_sharedContext), spawnPosition);
			}
			else if (entityName == "Player")
			{
				m_sharedContext.m_entityManager->add(new Player(m_sharedContext), spawnPosition);
			}
		}
	}
	file.close();
}

void Map::draw(sf::RenderWindow & window)
{
	if (m_backgroundName != "")
	{
		window.draw(m_backgroundImage);
	}
	
	//Change to only draw the tiles which are in the bounds of the view.
	sf::Vector2f viewCenter = m_sharedContext.m_window->getWindow().getView().getCenter();
	sf::Vector2f viewSize = m_sharedContext.m_window->getWindow().getView().getSize();
	viewSize += sf::Vector2f(64, 64); //Factor in for initial column of tiles to be loaded in. - Hacky :(
	sf::Vector2f viewSpace = viewCenter + (viewSize / 2.0f);
	//for (int y = (viewSpace.y - viewSize.y); y < viewSpace.y; ++y)
	//{
	//	for (int x = 0; x < viewSpace.x; ++x)
	//	{
	//		Tile* tile = getTile(std::floor(x), std::floor(y));
	//		if (tile)
	//		{
	//			sf::Vector2i& tilePos = tile->m_tileInfo->m_position;
	//			sf::Sprite& sprite = tile->m_tileInfo->m_sprite;
	//			sprite.setPosition(tilePos.x * Sheet::TILE_SIZE, tilePos.y * Sheet::TILE_SIZE);
	//			window.draw(sprite);
	//		}
	//	}
	//}

	for (const auto &i : m_tileMap)
	{
		sf::Vector2i& pos = i.second->m_tileInfo->m_position;
		if ((pos.x * Sheet::TILE_SIZE) >= (viewSpace.x - viewSize.x) &&
			(pos.x * Sheet::TILE_SIZE) <= (viewSpace.x) &&
			(pos.y * Sheet::TILE_SIZE) >= (viewSpace.y - viewSize.y) &&
			(pos.y * Sheet::TILE_SIZE) <= (viewSpace.y))
		{
			sf::Sprite* sprite = &i.second->m_tileInfo->m_sprite;
			sprite->setPosition(pos.x * Sheet::TILE_SIZE, pos.y * Sheet::TILE_SIZE);
			window.draw(*sprite);
		}
	}
}

void Map::update()
{
	if (m_nextLevel)
	{
		m_nextLevel = false;
		
		if (m_currentMapIndex >= static_cast<int>(MapNames::Total)) {
			return;
		}
		++m_currentMapIndex;
		//Find the next map to load
		auto cIter = m_maps.find(m_mapNames[m_currentMapIndex - 1]);
		if (cIter != m_maps.cend())
		{
			unloadCurrentMap();
			loadMap(cIter->second);
		}
		else {
			std::cerr << "Unable to find: " << cIter->second << "\n";
		}
	}

	updateBackgroundPosition();
}

Tile * Map::getTile(const unsigned int x, const unsigned int y)
{
	for (const auto &i : m_tileMap)
	{
		if (i.first == convertCoords(x, y))
		{
			return i.second;
		}
	}
	return nullptr;

	//auto iter = m_tileMap.find(convertCoords(x, y));
	//if (iter != m_tileMap.cend())
	//{
	//	return iter->second;
	//}
	//return nullptr;
}

void Map::unloadCurrentMap()
{
	m_sharedContext.m_entityManager->purgeEntities();
	m_tileMap.clear();
}

void Map::loadInTiles(const std::string & fileName)
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
		if (line[0] == '|') {
			continue;
		}
		
		unsigned int id = 0;
		std::string name;
		keyStream >> id >> name;
		if (!m_tileSet.emplace(id, new TileInfo(*m_sharedContext.m_textureManager, "TileSheet", id)).second)
		{
			std::cerr << "Duplicate tile detected." << id << "\n";
		}
	}
	file.close();
}

void Map::loadInMaps(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open()) {
		std::cerr << "Couldn't open: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string mapName;
		std::string mapLocation;
		keyStream >> mapName >> mapLocation;
		if (!m_maps.emplace(std::make_pair(mapName, mapLocation)).second) 
		{
			std::cerr << "Failed to load '" << mapName << "' '" << mapLocation << "' " << "\n";
			continue;
		}
	}
	file.close();
}

void Map::updateBackgroundPosition()
{
	sf::Vector2f currentPos = m_backgroundImage.getPosition();
	sf::View view = m_sharedContext.m_window->getWindow().getView();

	m_backgroundImage.setPosition(view.getCenter().x - (view.getSize().x / 2.0f), currentPos.y);
}

void Map::purgeTileSet()
{
	for (auto &i : m_tileSet)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_tileSet.clear();
}

void Map::purgeTileMap()
{
	for (auto &i : m_tileMap)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_tileMap.clear();
}