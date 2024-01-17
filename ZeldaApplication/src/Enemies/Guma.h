#ifndef GUMA_HPP
#define GUMA_HPP

#include "Enemy.h"

class Guma : public Enemy {

public:
    Guma();
    ~Guma() = default;
    void throwObject();
};

#endif 
