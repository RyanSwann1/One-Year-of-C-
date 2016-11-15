#pragma once
#include "Computer.h"
#include "Timer.h"
class Rat :
	public Computer
{
public:
	Rat(const SharedContext& sharedContext);
	~Rat();

	void update(const float deltaTime) override;
	void onEntityCollision(Entity& entity) override;

private:
	bool m_attackReady;
	Timer m_attackTimer;

	void handleAttack(const float deltaTime);
	void handleMovement();
};

