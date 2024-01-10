#include "Animator.h"
#include <Engine/Renderer/AnimatorManager.h>  
#include <Engine/Scene/Scene.h>

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

	Animator::Animator(Scene* scene) 
		: scene(scene)
	{  
		scene->GetAnimatorManager().Reginster(this);
	}

	Animator::~Animator()
	{ 
		scene->GetAnimatorManager().Cancel(this);
	}

	void Animator::Stop(void)
	{ 
		Finish(true);
	} 

	void Animator::NotifyStopped(void)
	{  
		scene->GetAnimatorManager().MarkAsSuspended(this);
		if (onFinish)
			(onFinish)(this);
	}

	void Animator::NotifyStarted(void)
	{ 
		scene->GetAnimatorManager().MarkAsRunning(this);
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

