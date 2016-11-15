#pragma once
#include "Character.h"
class Computer :
	public Character
{
public:
	Computer(const SharedContext& sharedContext);
	~Computer();

	void update(const float deltaTime);

private:
	void resolveCollisions() override;
};

