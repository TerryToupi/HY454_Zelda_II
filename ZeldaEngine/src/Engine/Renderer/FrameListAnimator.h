#pragma once 

#include <Engine/Renderer/Animator.h> 
#include <Engine/Renderer/MovingAnimation.h> 

namespace Engine
{
	class FrameListAnimator : public Animator
	{
	public:
		FrameListAnimator(void) = default;

		void		Progress(Time currtime);
		uint32_t	GetCurrFrame(void) const { return m_currFrame; }
		uint32_t	GetCurrRep(void) const { return m_currRep; }
		void		Start(FrameListAnimation* a, Time t)
		{
			m_anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currFrame = a->GetFrames().at(0);
			m_currRep = 0;
			NotifyStarted();
			NotifyAction(*m_anim);
		}
	private:
		FrameListAnimation*	 m_anim = nullptr;
		uint32_t			 m_currFrame = 0;
		uint32_t			 m_currRep = 0;
		uint32_t			 m_frameIndex = 0;
	};
}
