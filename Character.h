#pragma once

#include "Direction.h"
#include "Entity.h"
#include "AnimationName.h"
#include "Timer.h"
#include <iostream>

class Character :
	public Entity
{
public:
	Character(const SharedContext& sharedContext);
	~Character();

	const int getDamageValue() { return m_damage; }
	EntityState getState() const { return m_currentState; }
	const int getLives() { return m_lives; }

	void move(const Direction dir);
	inline void setState(const EntityState newEntityState) { m_currentState = newEntityState; }
	inline void setDirection(const Direction newDir)
	{
		if (m_currentDirection != newDir)
		{
			m_currentDirection = newDir;
			m_velocity.x = 0;
		}
	}

	void update(const float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
	void reduceLife(const int damage);

protected:
	int m_lives;
	int m_maxLives;
	int m_damage;
	float m_jumpVelocity;
	float m_jumpTime;
	float m_hurtTime;
	bool m_jumpReady;
	bool m_hurt;
	EntityState m_currentState;
	sf::FloatRect m_attackAABB;
	Timer m_jumpingTimer;
	Timer m_hurtTimer;
	
	void load(const std::string& fileName) override;
	void attackPreperation();
	void jump();
	inline void changeDirection()
	{
		if (m_currentDirection == Direction::Left) {
			m_currentDirection = Direction::Right;
		}
		else if (m_currentDirection == Direction::Right) {
			m_currentDirection = Direction::Left;
		}
	}
	void killCharacter(); //Set to destroy this character

	void resolveCollisions() override;

private:
	void attack(Entity& entity, const int x, const int y);
	void handleAnimations();
	void handleTimers(const float deltaTime);
	sf::RectangleShape m_attackShape;
	std::vector<sf::RectangleShape> m_attackShapes;
};

