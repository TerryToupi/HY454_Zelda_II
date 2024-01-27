#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

class Door : public Entity
{
public:
	Door(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene);
	~Door() = default;
};
