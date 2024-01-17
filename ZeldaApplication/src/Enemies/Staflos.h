#ifndef STAFLOS_HPP
#define STAFLOS_HPP

#include "Enemy.h"

class Staflos : public Enemy {

public:
    Staflos();
    ~Staflos() = default;
    void apearFromCeiling();

};

#endif 
