#pragma once 

#include <Engine/Renderer/Animator.h> 
#include <Engine/Renderer/MovingPathAnimation.h> 

namespace Engine
{
	class MovingPathAnimator : public Animator
	{
	public:
		MovingPathAnimator(Scene* scene);

		void		Progress(Time currtime);
		uint32_t	GetCurrRep(void) const { return m_currRep; } 
		uint32_t	GetCurrPath(void) const { return m_currPath; }
		void		Start(MovingPathAnimation * a, Time t)
		{
			m_anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING;
			m_currRep = 0; 
			m_currPath = 0;
			NotifyStarted();
			NotifyAction(*m_anim);
		}
	private:
		MovingPathAnimation*  m_anim = nullptr;
		uint32_t			 m_currRep = 0; 
		uint32_t			 m_currPath = 0;
	};
}
