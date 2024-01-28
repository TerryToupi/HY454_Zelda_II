#include "Application.h" 
#include "Layer.h"  
#include <Engine/Assert/Assert.h>
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Renderer/Render.h> 
#include <Engine/Input/KeyBoardCodes.h>
#include <Engine/Renderer/AnimatorManager.h>


namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfig& config) 
		:	m_AppConfig(config)
	{  
		ENGINE_CORE_ASSERT(!s_Instance); 
		s_Instance = this; 
		
		uint64_t default_w = 640;
		uint64_t default_h = 480;
		m_Window = Window::Create(WindowConfig(m_AppConfig.appName, default_w, default_h));  
		m_Window->SetEventCallBack(EVENT_FUNCTION_BIND(Application::onEvent));
		
		m_Running = true;  

		AudioManager::Get().Init();
	
		RendererConfig rConfig; 
		rConfig.fb_height = default_h;
		rConfig.fb_width = default_w; 
		Renderer::Init(rConfig); 
		ENGINE_CORE_INFO("Application successfully initialized!");  
	} 

	Application::~Application()
	{
		ENGINE_CORE_WARN("Shutting down UserCode");
		DestructionManager::Get().Commit();

		ENGINE_CORE_WARN("Shutting down AudioManager"); 
		AudioManager::Get().Shutdown();

		ENGINE_CORE_WARN("Shutting down Renderer"); 
		Renderer::Shutdown(); 

		ENGINE_CORE_WARN("Shutting down Application"); 
	}

	void Application::onEvent(Event& e)
	{ 
		EventDispatcher dispatcher(e); 
		dispatcher.Dispatch<WindowCloseEvent>(EVENT_FUNCTION_BIND(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EVENT_FUNCTION_BIND(Application::OnWindowResize)); 
		dispatcher.Dispatch<WindowPauseEvent>(EVENT_FUNCTION_BIND(Application::OnWindowPause));

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
		layer->onStart();
		s_Instance->m_Layers.pushBackLayer(layer); 
	}

	void Application::pushOverLay(Layer* Overlay)
	{  
		Overlay->onStart();
		s_Instance->m_Layers.pushBackOverLay(Overlay);
	}

	Layer& Application::GetLayer(std::string tag)
	{
		return *(s_Instance->m_Layers.GetLayer(tag));
	}

	Layer& Application::GetOverlay(std::string tag)
	{
		return *(s_Instance->m_Layers.GetOverlay(tag));
	}

	void Application::popLayer(Layer* layer)
	{	 
		s_Instance->m_Layers.popBackLayer(layer);
	}

	void Application::popOverLay(Layer* overlay)
	{ 
		s_Instance->m_Layers.popBackOverLay(overlay);
	}

	void Application::Run()
	{  
		Time currTime = SystemClock::Get().GetTime();
		Time prevTime = currTime;  
		Time statTimer = currTime;
		Time timeStep; 

		while (m_Running)
		{  
			currTime = SystemClock::Get().GetTime(); 
			timeStep = currTime - prevTime;    
			prevTime = currTime; 
			SystemClock::SetDeltaTime(currTime); 
			SystemClock::SetDeltaTimeStep(timeStep);

			if (currTime >= statTimer + 150)
			{ 
				Application::Instance().GetWindow().UpdateEngineStats(timeStep);
				statTimer = currTime;
			}

			Application::Instance().GetWindow().EventPolling();
			KeyboardInput::UpdateKeyState(); 

			if (!IsPaused())
			{
				DestructionManager::Get().Commit();
				for (auto layer = m_Layers.LayersFront(); layer != m_Layers.LayersBack(); layer++)
				{
					(*layer)->onUpdate(timeStep);
				}

				for (auto overlay = m_Layers.OverlaysFront(); overlay != m_Layers.OverLaysBack(); overlay++)
				{
					(*overlay)->onUpdate(timeStep);
				}
				AnimatorManager::GetInstance().Progress(currTime);
			}

			Renderer::BufferFlip(); 
		}
	} 

	void Application::SetOnPauseFunction(const PauseAction& f)
	{ 
		s_Instance->m_pauseFunction = f;
	}

	void Application::SetOnResumeFunction(const PauseAction& f)
	{ 
		s_Instance->m_resumeFunction = f;
	}

	void Application::Pause(Time t)
	{ 
		m_isPaused = true; 
		m_pauseTimestamp = t; 
		if (m_pauseFunction)
			m_pauseFunction();
	}

	void Application::Resume(void)
	{ 
		m_isPaused = false;  
		if (m_resumeFunction)
			m_resumeFunction(); 
	}

	bool Application::IsPaused(void)
	{
		return m_isPaused;
	}

	Time Application::GetPauseTime(void) const
	{
		return m_pauseTimestamp;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{ 
		m_Running = false; 
		return true;
	} 

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{   
		Application::Instance().GetWindow().ResizeWindow(e.GetWidth(), e.GetHeight());
		return true;
	} 

	bool Application::OnWindowPause(WindowPauseEvent& e)
	{ 
		if (IsPaused())
		{
			AnimatorManager::GetInstance().TimeShift(SystemClock::GetDeltaTime() - GetPauseTime());
			Resume(); 
		}
		else
		{
			Pause(SystemClock::GetDeltaTime()); 
		}

		return true;
	}
}
