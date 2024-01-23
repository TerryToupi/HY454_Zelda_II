#ifndef ENEMY_HPP
#define ENEMY_HPP

#include<Engine.h>
#include"../Entity/Entity.h"

class Enemy : public Entity {
private:
    int health;
    int damage;
    int points;

protected:
    uint32_t m_stage;
    uint32_t m_id;

public:
    Enemy() = default;
    ~Enemy() = default;
    int GetHealth() const;
    int GetDamage() const;
    int GetPoints() const;
    int GetStage() const;
    uint32_t GetID() const;

    void SetHealth(int newHealth);
    void SetDamage(int newDamage);
    void SetPoints(int newPoints);
    

 
    void Attack();
    void TakeDamage(int amount);
    void Disapear();
    void Move();
};

#endif 
