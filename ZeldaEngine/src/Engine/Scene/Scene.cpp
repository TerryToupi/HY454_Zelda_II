#include "Scene.h" 

#include <Engine/Scene/Sprite.h> 
#include <Engine/Scene/SpriteData.h>

namespace Engine {
	Scene::Scene(uint32_t layerid)
	{ 
		m_Tiles = MakeReference<TileLayer>(layerid);
	}

	Sprite Scene::CreateSprite(std::string tag)
	{ 
		m_SpritesMap.emplace(tag, SpriteData(tag)); 
		return { tag, this };
	} 

	Sprite Scene::GetSprite(std::string tag)
	{
		return { tag, this };
	}
}
