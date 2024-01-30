#pragma once

#include<Engine.h>
#include "../Entity/Entity.h"
#include <json.hpp>

using json = nlohmann::json;

class Flash : public Entity {

    public:
        Flash();
        ~Flash() = default;
};
