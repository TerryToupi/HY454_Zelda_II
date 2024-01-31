#ifndef STAFLOS_HPP
#define STAFLOS_HPP

#include "Enemy.h"

class Staflos : public Enemy {

public:
    Staflos(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Staflos() = default;
    void SetSleeping(bool _sleeping);
    bool isSleeping();

    void SetAttackCooldown(int32_t _cooldown);
    int32_t GetAttackCooldown();

private:
    bool m_sleeping;
    int32_t m_attackCooldown;

};

#endif 
