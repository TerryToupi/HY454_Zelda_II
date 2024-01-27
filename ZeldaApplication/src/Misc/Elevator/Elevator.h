#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

using namespace Engine;

class Elevator : public Entity {
public:
	Elevator();
	~Elevator() = default;

	void SetSecondary(Sprite _secondary);
	Sprite GetSecondary();

private:
	Sprite m_secondary;
};
