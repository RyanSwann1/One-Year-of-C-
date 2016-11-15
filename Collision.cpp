#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::boundingBoxCollision(const sf::FloatRect box1, const sf::FloatRect box2)
{
	//return the intersection amount
	//If bounding boxes are colliding
	//if((box1.left <= (box2.left + box2.width)) && //Check left
	//	((box1.left + box1.width) + box2.left) && //Check right
	//	(box1.top <= (box2.top + box2.height)) && //Check top
	//	((box1.top + box1.height) >= box2.top))
	//{
	//	return true;
	//}
	if ((box1.left <= (box2.left + box2.width)))
	{
		return true;
	}
	else if (((box1.left + box1.width) + box2.left))
	{
		return true;
	}
	else if ((box1.top <= (box2.top + box2.height)))
	{
		return true;
	}
	else if (((box1.top + box1.height) >= box2.top))
	{
		return true;
	}
	else {
		return false;
	}
}
