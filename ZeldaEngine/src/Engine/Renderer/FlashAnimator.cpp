#include "FlashAnimator.h"

namespace Engine
{
	FlashShowAnimator::FlashShowAnimator(Scene* scene)
		: Animator(scene)
	{ 
	} 

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

	FlashHideAnimator::FlashHideAnimator(Scene* scene)
		: Animator(scene)
	{
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
