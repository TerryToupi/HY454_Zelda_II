#pragma once 

#include<Engine/Renderer/Animator.h> 
#include<set>

namespace Engine
{  
	class AnimatorManager
	{
	public:
		void Reginster(Animator* a); 
		void Cancel(Animator* a); 
		void MarkAsRunning(Animator* a); 
		void MarkAsSuspended(Animator* a); 
		void Progress(Time currTime); 
		void TimeShift(Time dt);

		static auto GetInstance(void) -> AnimatorManager& { return s_Instance; } 
		static auto GetInstanceConst(void) -> const AnimatorManager& { return s_Instance; }
	
	private:
		AnimatorManager(void) = default;  
		AnimatorManager(const AnimatorManager&) = delete; 
		AnimatorManager(AnimatorManager&&) = delete;

	private: 
		std::set<Animator*>	running, suspended; 

	private:
		static AnimatorManager s_Instance;
	};
} 

