#include "Spell.h"

int Spell::getCost() const {
    return cost;
}

int Spell::getDuration() const {
    return duration;
}

int Spell::getCooldown() const {
    return cooldown;
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

