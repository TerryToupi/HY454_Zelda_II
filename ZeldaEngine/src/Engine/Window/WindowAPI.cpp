#include "WindowAPI.h"

#include <Engine/Events/ApplicationEvents.h> 
#include <Engine/Events/KeyBoardEvents.h>

namespace Engine 
{
	WindowAPI::WindowAPI(const WindowConfig& config)
	{ 
		Init(config);
	} 

	WindowAPI::~WindowAPI()
	{ 
		ShutDown();
	} 

	void WindowAPI::EventPolling()
	{ 
		SDL_Event sdlEvent; 
		
		if (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT)
			{
				WindowCloseEvent event;  
				m_EventCallBack(event);
			} 

			else if (sdlEvent.type == SDL_WINDOWEVENT)
			{
				if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					ENGINE_CORE_TRACE("Window resized!");
				}
			} 

			else if (sdlEvent.type == SDL_KEYDOWN)
			{
				if (sdlEvent.key.repeat)
				{
					KeyRepeatEvent event(sdlEvent.key.repeat);   
					ENGINE_CORE_TRACE("SDL key pressed Event with keycode {0}", sdlEvent.key.keysym.sym);
					m_EventCallBack(event);
				} 
				else
				{
					KeyTapEvent event; 
					ENGINE_CORE_TRACE("SDL key tapped Event with keycode {0}", sdlEvent.key.keysym.sym);
					m_EventCallBack(event);
				}
			} 

			else if (sdlEvent.type == SDL_KEYUP)
			{ 
				KeyReleaseEvent event;
				ENGINE_CORE_TRACE("SDL key released Event with keycode {0}", sdlEvent.key.keysym.sym);
				m_EventCallBack(event);
			} 
		}
	}

	void WindowAPI::Init(const WindowConfig& config)
	{ 
		m_Data.Height = config.Height;
		m_Data.Width = config.Width;
		m_Data.TitleBar = config.WindowName;  

		int status = SDL_Init(SDL_INIT_VIDEO);
		ENGINE_CORE_ASSERT(!status); 
	
		Uint32 WindowFalgs = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS;

		m_Window = SDL_CreateWindow(
			m_Data.TitleBar.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_Data.Width, m_Data.Height,
			WindowFalgs
		); 
		ENGINE_CORE_ASSERT(m_Window != nullptr); 
		ENGINE_CORE_INFO("Window created succesfully!");

		m_Renderer = SDL_CreateRenderer(
			m_Window, 
			-1, 
			SDL_RENDERER_PRESENTVSYNC
		); 
		ENGINE_CORE_ASSERT(m_Renderer != nullptr);
		ENGINE_CORE_INFO("Renderer created succesfully!"); 
	}

	void WindowAPI::ShutDown()
	{  
		SDL_DestroyRenderer(m_Renderer); 
		ENGINE_CORE_WARN("Shutting down SDL_Renderer"); 
		SDL_Delay(10);
		SDL_DestroyWindow(m_Window); 
		ENGINE_CORE_WARN("Shutting down SDL_Window"); 
		SDL_Delay(10); 
		SDL_Quit();
	}
}
