#ifndef LINK_HPP
#define LINK_HPP

#include <Engine.h>
#include "../Entity/Entity.h"

using namespace Engine;

class Link : public Entity{
private:
    int health;
    int damage;
    int magicPoints;
    int lives;
    int speed;
    int jumpingForce;

public:
    Link();
    ~Link() = default;

    int getHealth() const;
    int getDamage() const;
    int getMagicPoints() const;
    int getLives() const;
    int getSpeed() const;
    int getJumpingForce() const;
    
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setMagicPoints(int newMagicPoints);
    void setLives(int newLives);
    void setSpeed(int newLives);
    void setJumpingForce(int jumpingForce);



    void takeDamage(int amount);
    void heal(int amount);
    void castSpell(int spellCost);
    void loseLife();

};

#endif 
