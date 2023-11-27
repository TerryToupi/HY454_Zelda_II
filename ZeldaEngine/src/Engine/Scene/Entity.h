#pragma once 

#include "Engine/Core.h"
#include "Engine/UUID/UUID.h"   
#include "Engine/Assert/Assert.h"   
#include "Component.h"  
#include "entt.hpp" 

#include <vector>

namespace Engine 
{
	class Visitor; 
	class SceneGraph;

	class Entity 
	{   
	public:
		Entity();
		Entity(entt::entity Entity, SceneGraph* graph);
		Entity(const Entity& other) = default;
		~Entity();

		template<typename T, typename... Args>
		T& addComponent(Args&& ...args) 
		{
			T& component = m_Graph->m_EntityReg.emplace<T>(m_Entity, std::forward<Args>(args)...); 
			return component;
		} 

		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&& ...args) 
		{
			T& component = m_Graph->m_EntityReg.emplace_or_replace<T>(m_Entity, std::forward<Args>(args)...); 
			return component;
		}

		template<typename T>
		T& getComponent() 
		{  
			return m_Graph->m_EntityReg.get<T>(m_Entity);
		} 

		template<typename T> 
		void removeComponent() 
		{
			m_Graph->m_EntityReg.remove<T>(m_Entity);
		}

		const ID getID();
	
		void setParent(Entity* parent);
		Entity* getParent();  

		void pushChild(Entity* child); 
		Entity* getChildAtIndex(uint64_t index);
		uint64_t getTotalChildren();

		std::vector<Entity*>::iterator childBegin(); 
		std::vector<Entity*>::iterator childEnd(); 
		std::vector<Entity*>::const_iterator childBegin() const;
		std::vector<Entity*>::const_iterator childEnd()	const;

		void deleteChild(uint64_t index); 

		operator bool() const { return m_Entity != entt::null; }
		operator entt::entity() const { return m_Entity; }
		operator uint32_t() const { return (uint32_t)m_Entity; } 
		operator ID() { return getID(); } 
		
		// Visitor access
		void Accept(Visitor* visitor);

		void setVisited(bool visited);
		bool getVisited();

	private:  
		friend class SceneGraph; 
		uint64_t m_Index;

		entt::entity m_Entity{ entt::null };

		bool m_Visited;
		Entity* m_Parent; 
		std::vector<Entity*> m_Child;    
		
		SceneGraph* m_Graph;
	};
};
