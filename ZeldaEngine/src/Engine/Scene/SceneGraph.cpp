#include "Engine/Logging/Log.h"
#include "Engine/Assert/Assert.h"

#include "SceneGraph.h"
#include "Entity.h"

namespace Engine {
	SceneGraph::SceneGraph()
	{ 
		m_RootIndex = UINT64_C(0);
	} 

	void SceneGraph::Init()
	{
		Entity root = createRoot();
		return;
	}

	Entity SceneGraph::createEntity(const std::string& tag)
	{
		return createEntityWithUUID(UUID(), tag);
	}

	Entity SceneGraph::createEntityWithUUID(const ID id, const std::string& tag)
	{ 
		Entity entity = { m_EntityReg.create(), this };
		entity.addComponent<component_id>(UUID(id));
		auto& tagComponent = entity.addComponent<component_tag>();
		tagComponent.tag = tag.empty() ? "Entity" : tag;
		m_EntityMap[id] = entity;
		return entity;
	}

	void SceneGraph::addEntity(const ID ParentID, const ID id)
	{ 
		Entity* entity_heap = &m_EntityMap[id];
		ENGINE_CORE_ASSERT(entity_heap != nullptr ); 

		Entity* parent_heap = &m_EntityMap[ParentID];
		ENGINE_CORE_ASSERT(parent_heap != nullptr ); 

		entity_heap->setParent(parent_heap);
		parent_heap->pushChild(entity_heap); 
		ENGINE_CORE_WARN("inserting: {0} to {1}", entity_heap->getComponent<component_tag>().tag, parent_heap->getComponent<component_tag>().tag);
		return;
	}

	void SceneGraph::addEntityToRoot(const ID id)
	{ 
		Entity* root = &m_EntityMap[m_RootIndex]; 
		ENGINE_CORE_ASSERT(root != nullptr );

		Entity* entity_heap = &m_EntityMap[id];
		ENGINE_CORE_ASSERT(entity_heap != nullptr ); 

		entity_heap->setParent(root);
		root->pushChild(entity_heap);
		ENGINE_CORE_WARN("inserting: {0} to {1}", entity_heap->getComponent<component_tag>().tag, root->getComponent<component_tag>().tag);
		return;
	}

	Entity SceneGraph::getEntityByUUID(const ID id)
	{ 
		if (m_EntityMap.find(id) != m_EntityMap.end())
			return Entity{ m_EntityMap.at(id), this };

		return {};
	}

	void SceneGraph::removeEnitty(const ID id)
	{
		ENGINE_CORE_ASSERT(m_EntityMap.find(id) != m_EntityMap.end());
		Entity* ent = &m_EntityMap.at(id);
		Entity* Parent = ent->getParent(); 
		ENGINE_CORE_ASSERT(Parent != nullptr);
		uint64_t childIndex = ent->m_Index;

		if (!ent->m_Child.empty()) 
		{
			for (Entity* child : ent->m_Child) 
				Parent->pushChild(child);
		}  
	
		ENGINE_CORE_ASSERT(Parent->m_Child.size() >= childIndex);
		Parent->deleteChild(childIndex);
		depricatedDestroyEntity(id);
	}

	void SceneGraph::depricatedDestroyEntity(const ID id)
	{   
		Entity ent = getEntityByUUID(id);
		m_EntityMap.erase(id);
		m_EntityReg.destroy(ent);
	}

	Entity SceneGraph::createRoot()
	{
		Entity entity = { m_EntityReg.create(), this };
		entity.addComponent<component_id>(UUID());
		m_RootIndex = entity.getComponent<component_id>().uuid;
		auto& tagComponent = entity.addComponent<component_tag>();
		tagComponent.tag = "root";
		m_EntityMap[m_RootIndex] = entity; 

		return entity;
	}
}
