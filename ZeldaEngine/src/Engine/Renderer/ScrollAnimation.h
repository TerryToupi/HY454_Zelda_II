#pragma once 
 
#include <Engine/Renderer/Animation.h> 

namespace Engine
{
	struct ScrollEntry
	{
		int32_t	dx = 0;
		int32_t	dy = 0;
		uint32_t delay = 0;
	}; 

	class ScrollAnimation : public Animation
	{
	public:  
		using Scroll = std::vector<ScrollEntry>; 

	public:   
		ScrollAnimation(const std::string& _id, const Scroll& _scroll) 
			: Animation(_id), m_Scroll(_scroll) { }

		const Scroll& GetScroll(void) const { return m_Scroll; } 
		void		  SetScroll(const Scroll& p) { m_Scroll = p; } 
		Animation*    Clone(void) const override
		{
			return new ScrollAnimation(m_Id, m_Scroll);
		}

	private: 
		Scroll m_Scroll;
	};
}
