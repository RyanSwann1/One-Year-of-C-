#pragma once

#include "SharedContext.h"
class Entity;
class Camera
{
public:
	Camera(const SharedContext& sharedContext);
	~Camera();

	void update(Entity& playera);
	void reset();

private:
	SharedContext m_sharedContext;
};

