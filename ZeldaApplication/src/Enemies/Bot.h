#ifndef BOT_HPP
#define BOT_HPP

#include "Enemy.h"

class Bot : public Enemy {

public:
    Bot(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Bot() = default;

    int32_t GetJumpCooldown();
    void SetJumpCooldown(int32_t cooldown);

private:
    int32_t m_jumpCooldown;
};

#endif 
