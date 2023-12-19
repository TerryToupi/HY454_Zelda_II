#include "LatelyDestroyable.h"

namespace Engine
{
	void DestructionManager::Register(LatelyDestroyable* d)
	{ 
		ENGINE_CORE_ASSERT(!d->IsAlive());
		dead.push_back(d); 
		return;
	} 

	void DestructionManager::Commit(void)
	{ 
		for (auto* d : dead)
			d->Delete();
		dead.clear(); 
		return;
	}


	void LatelyDestroyable::Delete(void)
	{ 
		ENGINE_CORE_ASSERT(!dying); 
		dying = true; 
		delete this; 
		return;
	}
}

