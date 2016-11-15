#pragma once

#include <SFML\Graphics.hpp>

class Collision
{
public:
	Collision();
	~Collision();

	bool boundingBoxCollision(const sf::FloatRect box1, const sf::FloatRect box2);

private:

};

