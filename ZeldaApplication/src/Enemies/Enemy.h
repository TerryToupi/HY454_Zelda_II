#ifndef ENEMY_HPP
#define ENEMY_HPP

#include<Engine.h>
#include"../Entity/Entity.h"

class Enemy : public Entity {
private:
    int health;
    int damage;
    int points;
    int speed;
    int m_damageCoolDown;


protected:
    uint32_t m_stage;
    uint32_t m_maxX;
    uint32_t m_minX;

public:
    Enemy() = default;
    ~Enemy() = default;
    int GetHealth() const;
    int GetDamage() const;
    int GetPoints() const;
    int GetSpeed() const;
    int GetStage() const;
    int getDamageCoolDown() const;
    uint32_t GetMaxX() const;
    uint32_t GetMinX() const;

    void SetHealth(int newHealth);
    void SetDamage(int newDamage);
    void SetPoints(int newPoints);
    void SetSpeed(int newPoints);
    void SetMaxX(uint32_t _maxX);
    void SetMinX(uint32_t _minX);
    void setDamageCoolDown(int _cooldown);

    void Attack();
    void TakeDamage(int amount);
    void Disapear();
    void Move();
};

#endif 
