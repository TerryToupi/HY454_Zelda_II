#include "Sprite.h" 

#include <Engine/Scene/Scene.h> 
#include <Engine/Scene/SpriteData.h>

namespace Engine
{
	Sprite::Sprite(std::string name, Scene* scene) 
		:	m_HashName(name),	m_Scene(scene)
	{ 
	}

	void Sprite::SetMover(const Mover& move)
	{
		//TODO
	}

	const Rect Sprite::GetBox(void) const
	{  
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);

		Rect r;
		r.x = spData->x; 
		r.y = spData->y; 
		r.w = spData->frameBox.w;
		r.h = spData->frameBox.h;
		return r;
	}

	void Sprite::Move(int dx, int dy)
	{ 
		//TODO
	}

	void Sprite::SetPos(int _x, int _y)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 

		spData->x = _x;
		spData->y = _y;  
		return;
	} 

	void Sprite::SetZorder(unsigned z)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		spData->zorder = z; 
		return;
	}

	unsigned Sprite::GetZorder(void)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		return spData->zorder;
	}

	void Sprite::SetFrame(byte i)
	{  
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		
		if (i != spData->frameNo)
		{ 
			//TODO FILM
		}
	}

	byte Sprite::GetFrame(void) const
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);  
		return spData->frameNo;
	} 

	const std::string& Sprite::GetTypeId()
	{
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->typeID;
	}

	void Sprite::SetVisibility(bool v)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		spData->isVisible = v; 
		return;
	}

	bool Sprite::IsVisible(void) const
	{
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->isVisible;
	}

	
}

