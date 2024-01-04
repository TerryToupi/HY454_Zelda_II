#include "FlashAnimator.h"

namespace Engine
{
	void FlashShowAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_Anim->GetShowDelay())
		{
			lastTime += m_Anim->GetShowDelay();
			NotifyAction(*m_Anim);
			if (++m_CurrRep == m_Anim->GetReps())
			{
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}
	} 

	void FlashHideAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_Anim->GetHideDelay())
		{
			lastTime += m_Anim->GetHideDelay();
			NotifyAction(*m_Anim);
			if (++m_CurrRep == m_Anim->GetReps())
			{
				state = ANIMATOR_FINISHED;
				NotifyStopped();
				return;
			}
		}
	}
}
