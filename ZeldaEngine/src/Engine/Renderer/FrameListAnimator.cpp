#include "FrameListAnimator.h"

namespace Engine
{
	void FrameListAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_anim->GetDelay())
		{
			if (m_currFrame == m_anim->GetFrames().back())
			{
				ENGINE_CORE_ASSERT(m_anim->IsForEver() || m_currRep < m_anim->GetReps());
				m_currFrame = m_anim->GetFrames().at(0); 
				m_frameIndex = 0;
			}
			else
				m_currFrame = m_anim->GetFrames().at(m_frameIndex++);

			lastTime += m_anim->GetDelay();
			NotifyAction(*m_anim);

			if (m_currFrame == m_anim->GetFrames().back())
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
