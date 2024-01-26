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

	auto SpriteManager::find(Sprite _s)-> std::list<Sprite>::iterator
	{
		return std::find_if( 
			m_dpyList.begin(),
			m_dpyList.end(),
			[_s](const Sprite s)
			{
				return s->GetHashName() == _s->GetHashName();
			}
		);
	}

	void SpriteManager::Add(Sprite s)
	{ 
		if (s->GetFilm() != NONPRINTABLE)
			insert_ascending_zorder(s); 

		m_sprites[s->GetHashName()] = s; 
	} 

	Sprite SpriteManager::Get(std::string& _tag)
	{
		return m_sprites[_tag];
	}

	void SpriteManager::remove(Sprite s)
	{
		ENGINE_CORE_ASSERT(m_sprites.find(s->GetHashName()) != m_sprites.end());  
		auto dpySprite = find(s);
		m_sprites.erase(s->GetHashName());  
		m_dpyList.erase(dpySprite);
		s->Destroy();
	}
}

