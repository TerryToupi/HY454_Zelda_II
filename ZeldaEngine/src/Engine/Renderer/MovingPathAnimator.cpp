#include "MovingPathAnimator.h"

namespace Engine
{
	MovingPathAnimator::MovingPathAnimator(Scene* scene) 
		: Animator(scene)
	{
	} 

	void MovingPathAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_anim->GetPath().at(m_currPath).delay)
		{
			lastTime += m_anim->GetPath().at(m_currPath).delay;
			NotifyAction(*m_anim); 
			++m_currPath;
			if (++m_currRep == m_anim->GetPath().size())
			{
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}
	}
}

