#ifndef BOT_HPP
#define BOT_HPP

#include "Enemy.h"

class Bot : public Enemy {

public:
    Bot(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet);
    ~Bot() = default;
    void jump();

};

#endif 
