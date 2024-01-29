#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Scene/Tiles.h> 
#include <Engine/Renderer/AnimatorManager.h> 
#include <Engine/Scene/SpriteManager.h>
#include <Engine/Scene/CollisionChecker.h>

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
		void   RemoveSprite(Sprite _sprite); 
		bool   HasSprite(std::string _tag);

		Ref<TileLayer> GetTiles() { return m_Tiles; } 
		SpriteManager& GetSpriteManager() { return m_sprites; }
		CollisionChecker& GetColider() { return m_colider; }

	private: 
		friend class SpriteClass;
		
	private:   
		Ref<TileLayer> m_Tiles;  
		SpriteManager m_sprites;
		CollisionChecker m_colider;
	};
};
