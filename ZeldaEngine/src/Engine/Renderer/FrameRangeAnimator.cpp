#include "FrameRangeAnimator.h" 

namespace Engine
{
	void FrameRangeAnimator::Progress(Time currtime)
	{
		while (currtime > lastTime && (currtime - lastTime) >= m_anim->GetDelay())
		{
			if (m_currFrame == m_anim->GetEndFrame())
			{
				ENGINE_CORE_ASSERT(m_anim->IsForEver() || m_currRep < m_anim->GetReps());
				m_currFrame = m_anim->GetStartFrame();
			}
			else
				++m_currFrame;

			lastTime += m_anim->GetDelay();
			NotifyAction(*m_anim);

			if (m_currFrame == m_anim->GetEndFrame())
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

