#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Scene/Tiles.h> 
#include <Engine/Renderer/AnimatorManager.h> 
#include <Engine/Scene/SpriteManager.h>

#include <string>
#include <unordered_map> 

namespace Engine 
{  
	class Scene 
	{
	public: 
		Scene(uint32_t layerid);
		Scene(const Scene& other) = default; 

		Sprite CreateSprite(std::string _name, int _x, int _y, AnimationFilm* _film, const std::string& _typeid);
		Sprite GetSprite(std::string _tag);  

		Ref<TileLayer> GetTiles() { return m_Tiles; } 
		AnimatorManager& GetAnimatorManager() { return m_AnimManager; }
		SpriteManager& GetSpriteManager() { return m_sprites; }

	private: 
		friend class SpriteClass;
		
	private:   
		Ref<TileLayer> m_Tiles;  
		SpriteManager m_sprites;
		AnimatorManager m_AnimManager;
	};
};
