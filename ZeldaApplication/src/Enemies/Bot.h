#ifndef BOT_HPP
#define BOT_HPP

#include "Enemy.h"

class Bot : public Enemy {

public:
    Bot();
    ~Bot() = default;
    void jump();

};

#endif 
