#pragma once 

#include <Engine/Renderer/ScrollAnimation.h>
#include <Engine/Renderer/Animator.h> 

namespace Engine
{
	class ScrollAnimator : public Animator
	{
		ScrollAnimator(void) = default;

		void		Progress(Time currtime);
		uint32_t	GetCurrRep(void) const { return m_currRep; }
		uint32_t	GetCurrScroll(void) const { return m_currScroll; }
		void		Start(ScrollAnimation* a, Time t)
		{
			m_anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currRep = 0;
			NotifyStarted();
			NotifyAction(*m_anim);
		}
	private:
		ScrollAnimation*     m_anim = nullptr;
		uint32_t			 m_currRep = 0;
		uint32_t			 m_currScroll = 0;
	};
}
