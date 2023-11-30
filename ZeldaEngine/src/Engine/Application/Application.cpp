#include "Application.h" 
#include "Layer.h"  
#include "Engine/Assert/Assert.h"

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& config) 
		:m_AppConfig(config)
	{  
		ENGINE_CORE_ASSERT(!s_Instance); 
		s_Instance = this;
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
			}
		}
	}
}
