#ifndef LINK_HPP
#define LINK_HPP

#include <Engine.h>
#include "../Entity/Entity.h"

using namespace Engine;

class Link : public Entity{
private:
    int health = 100;
    int damage = 8;
    int magicPoints = 100;
    int lives = 3;

public:
    Link();
    ~Link() = default;

    int getHealth() const;
    int getDamage() const;
    int getMagicPoints() const;
    int getLives() const;
    
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setMagicPoints(int newMagicPoints);
    void setLives(int newLives);

    void takeDamage(int amount);
    void heal(int amount);
    void castSpell(int spellCost);
    void loseLife();

};

#endif 
