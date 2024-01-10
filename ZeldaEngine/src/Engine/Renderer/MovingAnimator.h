#pragma once 

#include <Engine/Renderer/Animator.h>  
#include <Engine/Renderer/MovingAnimation.h>

namespace Engine
{
	class MovingAnimator : public Animator
	{
	public:  
		MovingAnimator(Scene* scene);

		void	Progress(Time currtime); 
		auto	GetAnim(void) const -> const MovingAnimation& { return *m_Anim; } 
		void	Start(MovingAnimation* a, Time t) 
		{
			m_Anim	  = a;  
			lastTime  = t;
			state	  = ANIMATOR_RUNNING;
			m_CurrRep = 0; 
			NotifyStarted();
		}
	private: 
		MovingAnimation* m_Anim = nullptr;
		uint32_t		 m_CurrRep = 0;
	};
}
