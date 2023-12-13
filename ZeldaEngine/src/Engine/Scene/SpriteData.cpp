#include "SpriteData.h" 

namespace Engine
{
	SpriteData::SpriteData(const std::string& name)
		: m_Name(name), m_UUID()
	{  
		frameBox.x, frameBox.y = 0;
		frameBox.w, frameBox.h = 0;
	}  

	SpriteData::SpriteData(const std::string& name, ID uuid)
		: m_Name(name), m_UUID(uuid)
	{ 
		frameBox.x, frameBox.y = 0;
		frameBox.w, frameBox.h = 0;
	}
}