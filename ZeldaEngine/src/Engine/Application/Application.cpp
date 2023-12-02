#include "Application.h" 
#include "Layer.h"  
#include "Engine/Assert/Assert.h"

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& config) 
		:	m_AppConfig(config)
	{  
		ENGINE_CORE_ASSERT(!s_Instance); 
		s_Instance = this; 
		
		m_Window = Window::Create(WindowConfig(m_AppConfig.appName));  
		m_Window->SetEventCallBack(EVENT_FUNCTION_BIND(Application::onEvent));

		m_Running = true;
		ENGINE_CORE_INFO("Application successfully initialized!");
	} 

	Application::~Application()
	{
	}

	void Application::onEvent(Event& e)
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
		while (m_Running) 
		{  
			Application::Instance().GetWindow().EventPolling(); 

			for (auto layer = m_Layers.stackFront(); layer != m_Layers.stackBack(); layer++)
			{ 
				(*layer)->onUpdate();   
			}
		}
	}
}
