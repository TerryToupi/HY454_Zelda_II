#pragma once

#include "Engine/Core.h" 
#include "Engine/UUID/UUID.h"

namespace Engine {  
	class SceneGraph;
	class Entity;
	class Visitor;

	class DFS
	{
	public:
		DFS() = default;
		DFS(SceneGraph* graph, Visitor* visitor);
		
		void resetVisited();
		void dig( const ID id );
	private:
		Visitor* m_Visitor;
		SceneGraph* m_Graph;
	};
}

