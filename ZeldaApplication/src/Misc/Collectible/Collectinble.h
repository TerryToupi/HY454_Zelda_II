#pragma once

#include <Engine.h>
#include "../../Entity/Entity.h"

enum c_type{
	C_KEY,
	C_REDPOTION,
	C_BLUEPOTION,
	C_LINK,
	C_BASICPOINTS,
	C_STRONGERPOINTS
};

class Collectible : public Entity
{
public:
	Collectible(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene, c_type _type);
	~Collectible() = default;

	int getCooldown() const;
	void setCooldown(int newCost);

	c_type GetType();
private:
	int cooldown;
	c_type m_type;
};
