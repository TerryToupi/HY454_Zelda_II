#include "ScrollAnimator.h"

namespace Engine
{ 
	void ScrollAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_anim->GetScroll().at(m_currScroll).delay)
		{
			lastTime += m_anim->GetScroll().at(m_currScroll).delay;
			NotifyAction(*m_anim); 
			++m_currScroll;
			if (++m_currRep == m_anim->GetScroll().size())
			{
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}
	}
}

