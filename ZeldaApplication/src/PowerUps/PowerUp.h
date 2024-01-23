#pragma once

#include<Engine.h>
#include "json.hpp"

using json = nlohmann::json;

class PowerUp{
private:
    int value;

public:
    PowerUp() = default;
    ~PowerUp() = default;

    int getValue() const;

    void setValue(int newCost);
};

