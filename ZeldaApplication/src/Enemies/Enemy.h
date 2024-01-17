#ifndef ENEMY_HPP
#define ENEMY_HPP

#include<Engine.h>
#include"../Entity/Entity.h"

class Enemy : public Entity {
private:
    int health;
    int damage;
    int points;

public:
    Enemy() = default;
    ~Enemy() = default;
    int getHealth() const;
    int getDamage() const;
    int getPoints() const;

    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setPoints(int newPoints);

 
    void attack();
    void takeDamage(int amount);
    void disapear();
    void move();
};

#endif 
