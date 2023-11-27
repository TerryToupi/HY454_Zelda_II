#pragma once 

#include "Engine/Core.h"   
#include "SceneGraph.h"

namespace Engine 
{ 
	class Scene 
	{
	public: 
		Scene();
		Scene(const Scene& other) = default;
		

	public:
		SceneGraph *m_Graph;

	private:
		friend class SceneGraph;
	};
};
