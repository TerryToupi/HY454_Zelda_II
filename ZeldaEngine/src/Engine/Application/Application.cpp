#include "Application.h" 
#include "Layer.h"  
#include <Engine/Assert/Assert.h>
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Renderer/Render.h>

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& config) 
		:	m_AppConfig(config)
	{  
		ENGINE_CORE_ASSERT(!s_Instance); 
		s_Instance = this; 
		
		uint64_t default_w = 1024;
		uint64_t default_h = 720;
		m_Window = Window::Create(WindowConfig(m_AppConfig.appName, default_w, default_h));  
		m_Window->SetEventCallBack(EVENT_FUNCTION_BIND(Application::onEvent));
		
		m_Running = true; 
		m_LastFrameTime = 0;
		ENGINE_CORE_INFO("Application successfully initialized!"); 
	
		SystemClock::Init();
		RendererConfig rConfig; 
		rConfig.fb_height = default_h;
		rConfig.fb_width = default_w; 
		Renderer::Init(rConfig);
		
	} 

	Application::~Application()
	{ 
		SystemClock::Shutdown(); 
		ENGINE_CORE_WARN("Shutting down SystemTimer"); 
		Renderer::Shutdown(); 
		ENGINE_CORE_WARN("Shutting down Renderer"); 

		ENGINE_CORE_WARN("Shutting down Application"); 
	}

	void Application::onEvent(Event& e)
	{ 
		EventDispatcher dispatcher(e); 
		dispatcher.Dispatch<WindowCloseEvent>(EVENT_FUNCTION_BIND(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EVENT_FUNCTION_BIND(Application::OnWindowResize));

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
		Time time = SystemClock::Get().GetTime();  
		Time timeStep = time - m_LastFrameTime;
		m_LastFrameTime = timeStep;

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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{ 
		Renderer::ResizeFrameBuffer(e.GetWidth(), e.GetHeight());
		return true;
	}
}
