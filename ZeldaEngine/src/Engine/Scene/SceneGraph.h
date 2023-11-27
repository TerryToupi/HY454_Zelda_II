#pragma once 

#include "Engine/UUID/UUID.h" 
#include <unordered_map> 

namespace Engine 
{   
	class Entity; 
	
	class SceneGraph 
	{
	public:   
		SceneGraph();
		SceneGraph(const SceneGraph& other) = default;

		void Init();
		
		Entity createEntity(const std::string& tag); 
		Entity createEntityWithUUID(const ID id, const std::string& tag);

		void addEntity(const ID ParentID, const ID id);
		void addEntityToRoot(const ID id);

		Entity getEntityByUUID(const ID id);

		void removeEnitty(const ID id);

	public:
		ID m_RootIndex;
		std::unordered_map<ID, Entity> m_EntityMap; 

	private: 
		void depricatedDestroyEntity(const ID id);
		Entity createRoot();
			
	private:  
		friend class Entity;
	}; 
}
