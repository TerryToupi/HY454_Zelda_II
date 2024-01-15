#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Scene/Sprite.h>

namespace Engine
{
	class CollisionChecker 
	{ 
	public:
		using Action = std::function<void(Sprite s1, Sprite s2)>; 

	public: 
		void Register(Sprite s1, Sprite s2, const Action& f); 
		void Cancel(Sprite s1, Sprite s2); 
		void Check(void) const;
	
	protected: 
		using Entry = std::tuple<Sprite, Sprite, Action>;
		std::list<Entry> entries;
		auto Find(Sprite s1, Sprite s2) -> std::list<Entry>::iterator;
	};
}
