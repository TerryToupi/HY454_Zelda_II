#ifndef WOSU_HPP
#define WOSU_HPP

#include "Enemy.h"

class Wosu : public Enemy {
public:
	Wosu(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet, Ref<Scene> _scene);
	~Wosu() = default;
};

#endif 
