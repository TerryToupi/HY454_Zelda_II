#pragma once 

#include <Engine/Renderer/Animation.h>

namespace Engine
{ 
	class FlashAnimation : public Animation
	{ 
	public: 
		using Me = FlashAnimation; 

	public:
		Me&			SetReps(uint32_t n)		 { m_Reps = n; return *(this); } 
		uint32_t	GetReps(void) const		 { return m_Reps; } 
		Me&			SetHideDelay(uint32_t d) { m_HideDelay = d; return *(this); } 
		uint32_t	GetHideDelay(void) const { return m_HideDelay; } 
		Me&			SetShowDelay(uint32_t d) { m_ShowDelay = d; return *(this); } 
		uint32_t	GetShowDelay(void) const { return m_ShowDelay; } 
		Animation* Clone(void) const override
		{
			return new FlashAnimation(m_Id, m_Reps, m_ShowDelay, m_HideDelay);
		} 

		FlashAnimation(
			const std::string& _id,
			uint32_t n, 
			uint32_t show,
			uint32_t hide
		) : Animation(_id), m_Reps(n), m_HideDelay(hide), m_ShowDelay(show) { }
 
	private:  
		uint32_t m_Reps = 0; 
		uint32_t m_HideDelay = 0;
		uint32_t m_ShowDelay = 0;
	};
}
