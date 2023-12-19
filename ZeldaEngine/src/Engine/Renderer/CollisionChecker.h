#pragma once 

#include <Engine/Application/Core.h> 

namespace Engine
{  
	class Sprite;

	class CollisionChecker final 
	{
	public:
		using Action = std::function<void(Sprite* s1, Sprite* s2)>;
		static CollisionChecker singleton; 

	public:
		void Register(Sprite* s1, Sprite* s2, const Action& f)
		{
			entries.push_back(std::make_tuple(s1, s2, f));
		} 

		void Cancel(Sprite* s1, Sprite* s2);
		void Check(void) const; 

		static auto GetSingleton(void) -> CollisionChecker&
		{
			return singleton;
		} 

		static auto GetSingletonConst(void) -> const CollisionChecker&
		{
			return singleton;
		} 

	protected:
		using Entry = std::tuple<Sprite*, Sprite*, Action>;
		std::list<Entry> entries;
		auto Find(Sprite* s1, Sprite* s2) -> std::list<Entry>::iterator;
	};
}
