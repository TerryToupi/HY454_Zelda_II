#include "SpriteManager.h"

namespace Engine
{
	void SpriteManager::insert_ascending_zorder(Sprite s)
	{ 
		auto begin = m_dpyList.begin(); 
		auto end   = m_dpyList.end(); 

		while ((begin != end) && ((*begin)->GetZorder() > s->GetZorder()))
		{
			++begin;
		} 

		m_dpyList.insert(begin, s);
	}

	void SpriteManager::Add(Sprite s)
	{
		insert_ascending_zorder(s);
		if ((m_sprites.find(s->GetHashName()) != m_sprites.end()) &&
			(m_sprites[s->GetHashName()] == nullptr))
			m_sprites.erase(s->GetHashName());

		m_sprites[s->GetHashName()] = s; 
	} 

	Sprite SpriteManager::Get(std::string& _tag)
	{
		return m_sprites[_tag];
	}

}
