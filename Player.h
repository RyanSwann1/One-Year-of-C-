#pragma once
#include "Character.h"
#include "EventDetails.h"
class Player :
	public Character
{
public:
	Player(SharedContext& sharedContext);
	~Player();

	void onEntityCollision(Entity& entity) override;
	

private:
	sf::RectangleShape m_player;
	void reactToInput(const EventDetails& eventDetails);
	void resolveCollisions() override;
};

