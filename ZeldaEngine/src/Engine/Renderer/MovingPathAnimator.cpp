#include "MovingPathAnimator.h"

namespace Engine
{
	void MovingPathAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_anim->GetPath().at(m_currPath).delay)
		{
			if (m_currFrame == m_anim->GetPath().back().frame)
			{
				ENGINE_CORE_ASSERT(m_anim->IsForEver() || m_currRep < m_anim->GetReps());
				m_currFrame = m_anim->GetPath().at(0).frame; 
			}
			else
				m_currFrame = m_anim->GetPath().at(m_currPath).frame;

			lastTime += m_anim->GetPath().at(m_currPath).delay;
			NotifyAction(*m_anim); 
			++m_currPath;

			if (m_currFrame == m_anim->GetPath().back().frame)
			{
				if (!m_anim->IsForEver() && ++m_currRep == m_anim->GetReps())
				{
					state = ANIMATOR_FINISHED;
					NotifyStopped();
					return;
				}
			}
		}
	}
}

