#pragma once

#include "Engine/Core.h"  

namespace Engine{  
	class Entity;

	class Visitor
	{ 
	public: 
		virtual void visitEntity(Entity* entity) = 0;
	};
};

