#include "Computer.h"
#include "SharedContext.h"
#include "Map.h"
#include <iostream>
Computer::Computer(const SharedContext& sharedContext)
	: Character(sharedContext)
{
}


Computer::~Computer()
{
}

void Computer::update(const float deltaTime)
{
	Character::update(deltaTime);
	//check tile from map
	//have game go from one side to otehr
	//Camera follows
}

void Computer::resolveCollisions()
{
	Character::resolveCollisions();

	if (m_collisionManager.collidingOnXAxis())
	{
		changeDirection();
	}
	Tile* tile = m_collisionManager.getReferenceTile();
	if (tile)
	{
		if (tile->m_deadly)
		{
			changeDirection();
		}
	}
}
