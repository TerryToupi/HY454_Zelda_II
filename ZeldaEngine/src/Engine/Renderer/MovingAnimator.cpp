#include "MovingAnimator.h"

namespace Engine
{
	void MovingAnimator::Progress(Time currtime)
	{ 
		while (currtime > lastTime && (currtime - lastTime) >= m_Anim->GetDelay())
		{
			lastTime += m_Anim->GetDelay(); 
			NotifyAction(*m_Anim); 
			if (!m_Anim->IsForEver() && ++m_CurrRep == m_Anim->GetReps())
			{
				state = ANIMATOR_FINISHED; 
				NotifyStopped(); 
				return;
			}
		}
	}
}
