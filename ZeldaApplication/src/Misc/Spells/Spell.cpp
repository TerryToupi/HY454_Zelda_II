#include "Spell.h"


std::string Spell::getType() const {
    return type;
}

int Spell::getCost() const {
    return cost;
}

int Spell::getDuration() const {
    return duration;
}

int Spell::getCooldown() const {
    return cooldown;
}

int Spell::getDurationRemainingTime() const {
    return durationRemainingTime;
}

int Spell::getCooldownRemainingTime() const {
    return cooldownRemainingTime;
}




void Spell::setType(std::string newType) {
    type = newType;
}

void Spell::setCost(int newCost) {
    cost = newCost;
}

void Spell::setDuration(int newDuration) {
    duration = newDuration;
}

void Spell::setCooldown(int newCooldown) {
    cooldown = newCooldown;
}

void Spell::setDurationRemainingTime(int newDurationRemainingTime) {
    durationRemainingTime = newDurationRemainingTime;
}

void Spell::setCooldownRemainingTime(int newCooldownRemainingTime) {
    cooldownRemainingTime = newCooldownRemainingTime;
}



bool Spell::isActive() {
    return getDurationRemainingTime() != 0;
}

bool Spell::canUse() {
    return getCooldownRemainingTime() == 0;
}

