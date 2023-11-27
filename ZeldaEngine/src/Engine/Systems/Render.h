#pragma once

#include "Visitor.h"

namespace Engine  
{  
	class Scene;
	class Entity;

	class Render : public Visitor
	{ 
	public:   
		Render() = default;
		Render(Scene* graph); 

		void renderSceneGraph();  

	private:
		void visitEntity(Entity* entity);  

	private: 
		Scene* m_Scene;
	};
}

