#pragma once 

#include<Engine/Application/Core.h>  
#include<Engine/Application/GameTime.h> 

namespace Engine
{
	enum animatorstate_t
	{
		ANIMATOR_FINISHED	= 0, 
		ANIMATOR_RUNNING	= 1,
		ANIMATOR_STOPPED	= 2
	};  

	class Animation; 
	class AnimatorManager;

	class Animator
	{
	public:
		using OnFinish	= std::function<void(Animator*)>;
		using OnStart	= std::function<void(Animator*)>; 
		using OnAction	= std::function<void(Animator*, const Animation&)>;  

	public:  
		Animator(void); 
		Animator(const Animator&) = delete; 
		Animator(Animator&&) = delete; 
		virtual ~Animator();

		void			Stop(void); 
		bool			HasFinished(void) const { return state != ANIMATOR_RUNNING; } 
		virtual void	TimeShift(Time offset);
		virtual void	Progress(Time currTime) = 0; 

		template<typename T> void SetOnFinish(const T& f) { onFinish = f; } 
		template<typename T> void SetOnStart(const T& f) { onStart = f; } 
		template<typename T> void SetOnAction(const T& f) { onAction = f; } 

	protected: 
		Time				lastTime = 0; 
		animatorstate_t		state = ANIMATOR_FINISHED; 
		OnFinish			onFinish;
		OnStart				onStart;
		OnAction			onAction;
		void				NotifyStopped(void); 
		void				NotifyStarted(void); 
		void				NotifyAction(const Animation&); 
		void				Finish(bool isForced = false);
	}; 
}
