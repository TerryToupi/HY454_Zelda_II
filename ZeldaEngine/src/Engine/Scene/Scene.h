#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Scene/Sprite.h>
#include <Engine/Scene/SpriteData.h> 
#include <Engine/Scene/Tiles.h>

#include <string>
#include <unordered_map> 

namespace Engine 
{  
	class Scene 
	{
	public: 
		Scene(uint32_t layerid);
		Scene(const Scene& other) = default; 

		Sprite CreateSprite(std::string tag);  
		Sprite GetSprite(std::string tag);  

		TileLayer& GetTiles() { return *m_Tiles; }

	private: 
		friend class Sprite;
		
	private:   
		Reference<TileLayer> m_Tiles;
		std::unordered_map<std::string, SpriteData> m_SpritesMap; 
	};
};
