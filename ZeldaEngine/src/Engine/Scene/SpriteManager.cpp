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

	bool SpriteManager::Has(std::string& _tag)
	{
		return m_sprites.find(_tag) != m_sprites.end();
	}

	void SpriteManager::remove(Sprite s)
	{ 
		Sprite tmp = s;
		ENGINE_CORE_ASSERT(m_sprites.find(tmp->GetHashName()) != m_sprites.end());  
		auto dpySprite = find(tmp);
		m_sprites.erase(tmp->GetHashName());  
		m_dpyList.erase(dpySprite);
		tmp->Destroy();
	}
}

