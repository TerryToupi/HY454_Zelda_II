#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

class Cloud : public Entity
{
public:
	Cloud(uint32_t _id, AnimationSheet* _sheet, Ref<Scene> _scene, uint32_t cloudNum, uint32_t speed);
	~Cloud() = default;
};
