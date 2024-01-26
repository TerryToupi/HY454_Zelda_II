#pragma once 

#include<Engine/Application/Core.h>

namespace Engine
{
	class LatelyDestroyable; 

	class DestructionManager final { 
	public:
		void Register(LatelyDestroyable* d);
		void Commit(void);
		static auto Get(void) -> DestructionManager& { return singleton; } 

	private:
		std::list<LatelyDestroyable*> dead;
		static DestructionManager singleton;
	}; 

	class LatelyDestroyable { 
	public:
		LatelyDestroyable(void) = default; 
		bool IsAlive(void) const { return alive; }
		void Destroy(void) {
			if (alive) {
				alive = false;
				DestructionManager::Get().Register(this);
			}
		}

	protected:
		friend class DestructionManager;
		bool alive = true;
		bool dying = false;
		virtual ~LatelyDestroyable() { /*ENGINE_CORE_ASSERT(dying);*/  }
		void Delete(void);
	};
}
