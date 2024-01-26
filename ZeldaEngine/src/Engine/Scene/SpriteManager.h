#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Scene/Sprite.h>  

#include<unordered_map> 
#include<list>  

#define NONPRINTABLE	nullptr

namespace Engine
{ 
	class SpriteManager final
	{
	public:
		using SpriteList = std::list<Sprite>;  
		using SpriteHash = std::unordered_map<std::string, Sprite>;
		using TypeLists = std::unordered_map<std::string, SpriteList>; 

	public:
		void	Add(Sprite s); 
		Sprite	Get(std::string& _tag);
		void	remove(Sprite s);  

		auto GetDisplayList(void) -> SpriteList& 
				{ return m_dpyList; } 
		auto GetTypeList(const std::string& _type) -> SpriteList& 
				{ return m_types[_type]; } 

		static auto Instance(void) -> SpriteManager& 
				{ return s_Instance; } 
		static auto InstanceConst(void) ->const SpriteManager&
				{ return s_Instance; } 

	private:
		void insert_ascending_zorder(Sprite s); 
		auto find(Sprite s) -> std::list<Sprite>::iterator;

	private:
		SpriteList	m_dpyList;
		SpriteHash	m_sprites;
		TypeLists	m_types; 

	private: 
		static SpriteManager s_Instance;
	};
}
