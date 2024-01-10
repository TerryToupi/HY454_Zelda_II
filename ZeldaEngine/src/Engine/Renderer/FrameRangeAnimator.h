#pragma once 

#include <Engine/Renderer/MovingAnimation.h> 
#include <Engine/Renderer/Animator.h> 

namespace Engine
{
	class FrameRangeAnimator : public Animator
	{
	public:  
		FrameRangeAnimator(Scene* scene);

		void		Progress(Time currtime);
		uint32_t	GetCurrFrame(void) const { return m_currFrame; }  
		uint32_t	GetCurrRep(void) const { return m_currRep; }  
		void		Start(FrameRangeAnimation* a, Time t)
		{
			m_anim = a; 
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currFrame = a->GetStartFrame(); 
			m_currRep = 0;
			NotifyStarted(); 
			NotifyAction(*m_anim);
		}
	private: 
		FrameRangeAnimation* m_anim = nullptr; 
		uint32_t			 m_currFrame = 0;
		uint32_t			 m_currRep = 0;
	};
}
