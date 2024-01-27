#pragma once

#include<Engine.h>
#include"../../Entity/Entity.h"
#include <json.hpp>

using json = nlohmann::json;

class Spell : public Entity {
private:
    std::string type;
    int cost;
    int duration;
    int cooldown;
    int durationRemainingTime;
    int cooldownRemainingTime;

public:
    Spell() = default;
    ~Spell() = default;
    
    std::string getType() const;
    int getCost() const;
    int getDuration() const;
    int getCooldown() const;
    int getDurationRemainingTime() const;
    int getCooldownRemainingTime() const;

    void setType(std::string newType);
    void setCost(int newCost);
    void setDuration(int newDuration);
    void setCooldown(int newCooldown);
    void setDurationRemainingTime(int newDurationRemainingTime);
    void setCooldownRemainingTime(int newCooldownRemainingTime);


    bool isActive();
    bool canUse();

};

