#ifndef GUMA_HPP
#define GUMA_HPP

#include "Enemy.h"

class Guma : public Enemy {

public:
    Guma(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Guma() = default;

    void SetAttackCooldown(int32_t _cooldown);
    int32_t GetAttackCooldown();
private:
    int32_t m_attackCooldown;
};

#endif 
