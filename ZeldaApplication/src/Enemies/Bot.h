#ifndef BOT_HPP
#define BOT_HPP

#include "Enemy.h"

class Bot : public Enemy {

public:
    Bot(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Bot() = default;

    uint32_t GetJumpCooldown();
    void SetJumpCooldown(uint32_t cooldown);

private:
    uint32_t m_jumpCooldown;
};

#endif 
