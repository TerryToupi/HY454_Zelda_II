#include "AnimatorManager.h" 

#include <thread>

namespace Engine
{ 
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
		//std::vector<std::thread> threads;

		auto copied(running); 
		for (auto* a : copied)
		{  
			//threads.push_back(std::thread
			//	{ 
			//		[](Animator* a, Time currTime) -> void { a->Progress(currTime); }, 
			//		std::ref(a), 
			//		std::ref(currTime) 
			//	}); 

			a->Progress(currTime);
		} 

		//for (auto& thread : threads)
		//{
		//	if (thread.joinable())
		//		thread.join();
		//}
	}
}

