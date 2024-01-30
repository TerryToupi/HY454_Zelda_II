#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

class Block : public Entity
{
public:
	Block(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene);
	~Block() = default;

	void SetRespawnCooldown(int32_t _cooldown);
	int32_t GetRespawnCooldown();

	bool IsDead();
	void SetDead(bool _dead);

private:
	int32_t m_respawnCooldown;
	bool m_dead;
};
