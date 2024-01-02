#include "Animation.h"

namespace Engine
{
	Animation::Animation(const std::string& _id)
		: m_Id(_id)
	{
	} 

	const std::string& Animation::GetId(void)
	{ 
		return m_Id;
	}

	void Animation::SetId(const std::string& _id)
	{ 
		m_Id = _id;
	}
}

