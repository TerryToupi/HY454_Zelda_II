#include "Entity.h"
#include "Engine/Systems/Visitor.h"  
#include "SceneGraph.h"

namespace Engine 
{
	Entity::Entity()
	{
		this->m_Entity = entt::null; 
		this->m_Graph = nullptr;

		this->m_Visited = false;
		this->m_Parent = nullptr; 
		this->m_Index = 0;
	} 

	Entity::Entity(entt::entity Entity, SceneGraph* graph)
	{
		this->m_Entity = Entity; 
		this->m_Graph = graph;
		
		this->m_Visited = false;
		this->m_Parent = nullptr; 
		this->m_Index = 0;
	}

	Entity::~Entity()
	{ 
		m_Child.clear();
	}

	const ID Entity::getID()
	{
		return getComponent<component_id>().uuid; 
	}

	void Entity::setParent(Entity* parent)
	{ 
		this->m_Parent = parent;
	} 

	Entity* Entity::getParent()
	{
		return this->m_Parent;
	} 

	void Entity::pushChild(Entity* child) 
	{	
		child->m_Index = this->m_Child.size();
		this->m_Child.push_back(child);
	} 

	Entity* Entity::getChildAtIndex(uint64_t index)
	{
		return this->m_Child.at(index);
	}

	uint64_t Entity::getTotalChildren()
	{
		return m_Child.size();
	}

	std::vector<Entity*>::iterator Entity::childBegin()
	{
		return this->m_Child.begin();
	}

	std::vector<Entity*>::iterator Entity::childEnd()
	{
		return this->m_Child.end();
	}

	std::vector<Entity*>::const_iterator Entity::childBegin() const
	{
		return this->m_Child.begin();
	} 

	std::vector<Entity*>::const_iterator Entity::childEnd() const
	{
		return this->m_Child.end();
	}

	void Entity::deleteChild(uint64_t index)
	{
		this->m_Child.erase(this->m_Child.begin() + index);
	} 

	void Entity::Accept(Visitor* visitor)
	{ 
		visitor->visitEntity(this);
	}

	void Entity::setVisited(bool visited)
	{ 
		this->m_Visited = visited;
	} 

	bool Entity::getVisited()
	{
		return this->m_Visited;
	}
}