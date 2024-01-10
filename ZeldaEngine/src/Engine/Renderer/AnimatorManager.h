#pragma once 

#include<Engine/Renderer/Animator.h> 
#include<set>

namespace Engine
{  
	class AnimatorManager 
	{
	public:
		AnimatorManager(void) = default;  

		void Reginster(Animator* a); 
		void Cancel(Animator* a); 
		void MarkAsRunning(Animator* a); 
		void MarkAsSuspended(Animator* a); 
		void Progress(Time currTime); 

	private: 
		std::set<Animator*>	running, suspended; 
		AnimatorManager(const AnimatorManager&) = delete; 
		AnimatorManager(AnimatorManager&&) = delete;
	};
} 

