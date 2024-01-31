#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

class Arrow : public Entity
{
public:
	Arrow(uint32_t _id, AnimationSheet* _sheet, Ref<Scene> _scene);
	~Arrow() = default;

	void SetCollided(bool _collided);
	bool IsCollided();

private:
	bool m_collided;
};
