#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

using namespace Engine;

class Elevator : public Entity {
public:
	Elevator(ID _id, AnimationSheet *_sheet, Ref<Scene> _scene);
	~Elevator() = default;
};
