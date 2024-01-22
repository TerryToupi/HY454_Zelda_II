#include "AnimatorManager.h" 

#include <thread>

namespace Engine
{  
	AnimatorManager AnimatorManager::s_Instance;

	void AnimatorManager::Reginster(Animator* a)
	{  
		ENGINE_CORE_ASSERT(a->HasFinished());
		suspended.insert(a);
	} 

	void AnimatorManager::Cancel(Animator* a)
	{
		ENGINE_CORE_ASSERT(a->HasFinished()); 
		suspended.erase(a);
	} 

	void AnimatorManager::MarkAsRunning(Animator* a)
	{
		ENGINE_CORE_ASSERT(!a->HasFinished());
		suspended.erase(a); 
		running.insert(a);
	} 

	void AnimatorManager::MarkAsSuspended(Animator* a)
	{
		ENGINE_CORE_ASSERT(a->HasFinished());
		running.erase(a);
		suspended.insert(a); 
	} 

	void AnimatorManager::Progress(Time currTime)
	{ 
		auto copied(running); 
		for (auto* a : copied)
		{  
			a->Progress(currTime);
		} 
	}

	void AnimatorManager::TimeShift(Time dt)
	{ 
		for (auto* a : running)
			a->TimeShift(dt);
	}
}

