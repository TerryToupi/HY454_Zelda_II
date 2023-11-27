#include "DFS.h" 
#include "Engine/Logging/Log.h"
#include "Engine/Scene/Entity.h" 
#include "Engine/Scene/SceneGraph.h" 
#include "Visitor.h"

#include <unordered_map>

namespace Engine {
	DFS::DFS(SceneGraph* graph, Visitor* visitor)
	{
		this->m_Graph = graph; 
		this->m_Visitor = visitor; 
	}

	void DFS::resetVisited()
	{ 
		for (auto entity : m_Graph->m_EntityMap) 
			if (entity.second.getVisited())
				entity.second.setVisited(false);
	} 

	void DFS::dig(const ID id) 
	{ 
		Entity* ent = &m_Graph->m_EntityMap[id]; 
		if (!ent->getVisited()) {
			ent->setVisited(true);  
			for (int i = 0; i < ent->getTotalChildren(); i++) 
			{
				Entity *entity = ent->getChildAtIndex(i);  
				if (!entity->getVisited()) 
				{
					dig(entity->getID());  
					entity->Accept(m_Visitor);
				}
			}
		}
	}
}

