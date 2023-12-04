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
		ENGINE_CORE_WARN("Shutting down Application");
	}

	void Application::onEvent(Event& e)
	{ 
		EventDispatcher dispatcher(e); 
		dispatcher.Dispatch < WindowCloseEvent > (EVENT_FUNCTION_BIND(Application::OnWindowClose)); 

		for (auto overlay = m_Layers.rOverlaysFront(); overlay != m_Layers.rOverLaysBack(); overlay++)
		{
			(*overlay)->onEvent(e);
		}
		for (auto layer = m_Layers.rLayersFront(); layer != m_Layers.rLayersBack(); layer++)
		{
			(*layer)->onEvent(e);
		}
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

			for (auto layer = m_Layers.LayersFront(); layer != m_Layers.LayersBack(); layer++)
			{ 
				(*layer)->onUpdate();   
			}
		}
	} 

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{ 
		m_Running = false; 
		return true;
	}
}
