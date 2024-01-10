#include "Scene.h" 

namespace Engine {
	Scene::Scene(uint32_t layerid)
	{ 
		m_Tiles = MakeReference<TileLayer>(layerid);
	}

	Sprite Scene::CreateSprite(std::string _name, int _x, int _y, AnimationFilm* _film, const std::string& _typeid)
	{ 
		Sprite s = new SpriteClass(_name, _x, _y, _film, _typeid); 
		m_sprites.Add(s);
		return s;
	}

	Sprite Scene::GetSprite(std::string _tag)
	{
		if (!m_sprites.Get(_tag))
			ENGINE_CORE_WARN("Sprite: {0}, has null data", m_sprites.Get(_tag)->GetHashName().c_str()); 

		return m_sprites.Get(_tag);
	}

	//SpriteClass Scene::CreateSprite(std::string tag)
	//{ 
	//	m_SpritesMap.emplace(tag, SpriteData(tag)); 
	//	return { tag, this };
	//} 

	//SpriteClass Scene::GetSprite(std::string tag)
	//{
	//	return { tag, this };
	//}
}
