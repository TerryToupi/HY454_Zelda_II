#ifndef WOSU_HPP
#define WOSU_HPP

#include "Enemy.h"

class Wosu : public Enemy {
public:
	Wosu(uint32_t _id, std::string direction);
	~Wosu() = default;
};

#endif 
