#include "Engine/Assert/Assert.h"
#include "Engine/Scene/Scene.h" 
#include "Engine/Scene/Entity.h"
#include "Render.h"  
#include "DFS.h"

namespace Engine 
{
	Render::Render(Scene* scene)
	{
		this->m_Scene = scene;
	} 

	void Render::visitEntity(Entity* entity)
	{ 
		auto& id = entity->getComponent<component_id>(); 
		auto& name = entity->getComponent<component_tag>(); 
		ENGINE_CORE_WARN("id: {0}, name: {1}",id.uuid,name.tag);
	} 

	void Render::renderSceneGraph()
	{ 
		DFS dfs = { m_Scene->m_Graph, (Visitor*)this };
		dfs.resetVisited();
		dfs.dig(m_Scene->m_Graph->m_RootIndex);
	}
}
