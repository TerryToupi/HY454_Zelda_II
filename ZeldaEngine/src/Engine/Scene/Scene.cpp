#include "Scene.h"

namespace Engine { 

	Scene::Scene()
	{ 
		this->m_Graph = new SceneGraph(); 
		this->m_Graph->Init();
	} 
}
