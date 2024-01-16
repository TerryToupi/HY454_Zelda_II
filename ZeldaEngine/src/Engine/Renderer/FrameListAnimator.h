#pragma once 

#include <Engine/Renderer/Animator.h> 
#include <Engine/Renderer/MovingAnimation.h> 

namespace Engine
{
	class FrameListAnimator : public Animator
	{
	public:
		FrameListAnimator() = default;

		void		Progress(Time currtime);
		uint32_t	GetCurrFrame(void) const { return m_currFrame; }
		uint32_t	GetCurrRep(void) const { return m_currRep; }
		void		Start(FrameListAnimation* a, Time t, uint32_t _currFrame)
		{
			m_anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currFrame = _currFrame;
			m_currRep = 0; 
			m_frameIndex = 0;
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
