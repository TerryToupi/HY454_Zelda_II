#pragma once

#include<Engine.h>
#include <json.hpp>

using json = nlohmann::json;

class Spell{
private:
    int cost;
    int duration;
    int cooldown;

public:
    Spell() = default;
    ~Spell() = default;
    
    int getCost() const;
    int getDuration() const;
    int getCooldown() const;

    void setCost(int newCost);
    void setDuration(int newDuration);
    void setCooldown(int newCooldown);

};

