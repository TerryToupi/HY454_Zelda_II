#pragma once 

#include <Engine/Renderer/Animator.h> 
#include <Engine/Renderer/MovingPathAnimation.h> 

namespace Engine
{
	class MovingPathAnimator : public Animator
	{
	public:
		MovingPathAnimator(void) = default;

		void		Progress(Time currtime);
		uint32_t	GetCurrFrame(void) const { return m_currFrame; }
		uint32_t	GetCurrRep(void) const { return m_currRep; }
		void		Start(MovingPathAnimation * a, Time t)
		{
			m_anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currFrame = a->GetPath().at(0).frame;
			m_currRep = 0;
			NotifyStarted();
			NotifyAction(*m_anim);
		}
	private:
		MovingPathAnimation*  m_anim = nullptr;
		uint32_t			 m_currFrame = 0;
		uint32_t			 m_currRep = 0; 
		uint32_t			 m_currPath = 0;
	};
}
