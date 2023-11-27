#include "Application.h" 
#include "Layer.h"

#include "Engine/Systems/Render.h"

namespace Engine {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::pushLayer(Layer* layer)
	{ 
		m_Layers.pushBackLayer(layer);
	}

	void Application::pushOverLay(Layer* Overlay)
	{ 
		m_Layers.pushBackOverLay(Overlay);
	}

	void Application::Run()
	{
		while (true) 
		{
			for (auto layer = m_Layers.stackFront(); layer != m_Layers.stackBack(); layer++)
			{ 
				(*layer)->onUpdate();   
				Render render = { (*layer)->m_Scene };
				render.renderSceneGraph();
			}
		}
	}
}
