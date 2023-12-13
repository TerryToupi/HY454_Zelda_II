#pragma once 

#include <Engine/Application/Core.h>

#include <string>
#include <unordered_map> 

namespace Engine 
{  
	class Sprite; 
	class SpriteData;
	
	class Scene 
	{
	public: 
		Scene() = default;
		Scene(const Scene& other) = default; 

		Sprite CreateSprite(std::string tag);  
		Sprite GetSprite(std::string tag); 

		
	private: 
		std::unordered_map<std::string, SpriteData> m_SpritesMap; 

	private: 
		friend class Sprite; 
		friend class SpriteData;
	};
};
