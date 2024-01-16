#pragma once 

#include <Engine/Renderer/Animator.h> 
#include <Engine/Renderer/FlashAnimation.h>

namespace Engine
{
	class FlashShowAnimator : public Animator
	{
	public:
		FlashShowAnimator() = default;

		void	 Progress(Time currtime);
		uint32_t GetShowDelay(void) const { return m_showDelay; }
		auto     GetAnim(void) const -> const FlashAnimation& { return *m_Anim; }
		void	 Start(FlashAnimation* a, Time t)
		{
			m_Anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING; 
			m_CurrRep = 0;
			NotifyStarted();
		}
	private:
		FlashAnimation* m_Anim = nullptr;
		uint32_t		m_CurrRep = 0;  
		uint32_t		m_showDelay = 0;
	}; 

	class FlashHideAnimator : public Animator
	{
	public:
		FlashHideAnimator() = default;

		void	 Progress(Time currtime);  
		uint32_t GetHideDelay(void) const { return m_hideDelay; }
		auto	 GetAnim(void) const -> const FlashAnimation& { return *m_Anim; }
		void	 Start(FlashAnimation* a, Time t)
		{
			m_Anim = a;
			lastTime = t;
			state = ANIMATOR_RUNNING; 
			m_CurrRep = 0;
			NotifyStarted();
		}
	private:
		FlashAnimation* m_Anim = nullptr;
		uint32_t		m_CurrRep = 0;  
		uint32_t		m_hideDelay = 0;
	};
}
