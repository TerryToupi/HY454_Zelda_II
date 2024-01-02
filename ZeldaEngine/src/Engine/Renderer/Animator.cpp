#include "Animator.h"
#include <Engine/Renderer/AnimatorManager.h> 

namespace Engine 
{
	void Animator::Finish(bool isForced)
	{  
		if (!HasFinished())
		{
			state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
			NotifyStopped();
		}
	} 

	Animator::Animator(void)
	{ 
		AnimatorManager::GetInstance().Reginster(this);
	}

	Animator::~Animator()
	{ 
		AnimatorManager::GetInstance().Cancel(this);
	}

	void Animator::Stop(void)
	{ 
		Finish(true);
	} 

	void Animator::NotifyStopped(void)
	{  
		AnimatorManager::GetInstance().MarkAsSuspended(this);
		if (onFinish)
			(onFinish)(this);
	}

	void Animator::NotifyStarted(void)
	{ 
		AnimatorManager::GetInstance().MarkAsRunning(this);
		if (onStart)
			(onStart)(this);
	}

	void Animator::NotifyAction(const Animation& anim)
	{
		if (onAction)
			(onAction)(this, anim);
	}  

	void Animator::TimeShift(Time offset)
	{
		lastTime += offset;
	}
}

