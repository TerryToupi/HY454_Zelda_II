#ifndef STAFLOS_HPP
#define STAFLOS_HPP

#include "Enemy.h"

class Staflos : public Enemy {

public:
    Staflos(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
    ~Staflos() = default;
    void apearFromCeiling();

};

#endif 
