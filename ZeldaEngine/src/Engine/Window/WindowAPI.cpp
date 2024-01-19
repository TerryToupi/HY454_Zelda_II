#include "WindowAPI.h"

#include <Engine/Events/ApplicationEvents.h> 
#include <Engine/Events/KeyBoardEvents.h> 
#include <Engine/Events/MouseEvents.h> 

#include <iostream>

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
					uint64_t Width = sdlEvent.window.data1;
					uint64_t Height = sdlEvent.window.data2;
					WindowResizeEvent event(Width, Height); 
					m_EventCallBack(event);
				}
			} 

			else if (sdlEvent.type == SDL_KEYDOWN)
			{
				if (sdlEvent.key.repeat)
				{
					InputKey key = (InputKey)sdlEvent.key.keysym.sym; 
					bool repeat = sdlEvent.key.repeat;
					KeyRepeatEvent event(key, repeat);   
					m_EventCallBack(event);
				} 
				else
				{ 
					InputKey key = (InputKey)sdlEvent.key.keysym.sym;
					KeyTapEvent event(key); 
					m_EventCallBack(event);
				}
			} 

			else if (sdlEvent.type == SDL_KEYUP)
			{ 
				InputKey key = (InputKey)sdlEvent.key.keysym.sym;
				KeyReleaseEvent event(key);
				m_EventCallBack(event);
			}

			else if (sdlEvent.type == SDL_MOUSEMOTION)
			{
				int x, y;
				SDL_GetMouseState(&x, &y); 
				MouseMotionEvent event(x, y);  
				m_EventCallBack(event);
			} 

			else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				MButton button = (MButton)sdlEvent.button.button;
				MouseClickPressEvent event(button);
				m_EventCallBack(event);
			} 

			else if (sdlEvent.type == SDL_MOUSEBUTTONUP)
			{ 
				MButton button = (MButton)sdlEvent.button.button;
				MouseClickReleaseEvent event(button);
				m_EventCallBack(event);
			} 

			else if (sdlEvent.type == SDL_MOUSEWHEEL)
			{ 
				int32_t x = sdlEvent.wheel.x;
				int32_t y = sdlEvent.wheel.y;
				MouseScrollEvent event(x, y);
				m_EventCallBack(event);
			}
		}
	}

	void* WindowAPI::GetKeyboardState() const
	{
		return (void*)(SDL_GetKeyboardState(NULL));
	}

	void WindowAPI::UpdateEngineStats(Time ts) const
	{ 
		std::string Tittle = m_Data.TitleBar;
		std::string FPS = std::to_string((int)((1.0 / ts) * 1000));
		std::string ms = std::to_string(ts);
		std::string newTittle = Tittle + " FPS: " + FPS + " CPU time: " + ms + "ms";
		SDL_SetWindowTitle(m_Window, newTittle.c_str());
	}

	void WindowAPI::Init(const WindowConfig& config)
	{ 
		m_Data.Height = config.Height;
		m_Data.Width = config.Width;
		m_Data.TitleBar = config.WindowName;  

		int status = SDL_Init(SDL_INIT_VIDEO);
		ENGINE_CORE_ASSERT(!status); 
	
		Uint32 WindowFalgs =  SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_OPENGL;
		Uint32 RendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

		m_Window = SDL_CreateWindow(
			m_Data.TitleBar.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_Data.Width, m_Data.Height,
			WindowFalgs
		);    
		
		m_Renderer = SDL_CreateRenderer(
			m_Window, 
			-1, 
			RendererFlags	
		);

		SDL_SetWindowMinimumSize(m_Window, m_Data.Width, m_Data.Height); 
		SDL_RenderSetLogicalSize(m_Renderer, m_Data.Width, m_Data.Height); 
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

		ENGINE_CORE_ASSERT(m_Window != nullptr);  
		ENGINE_CORE_ASSERT(m_Renderer != nullptr);
		ENGINE_CORE_INFO("Window created succesfully!"); 
	}

	void WindowAPI::ShutDown()
	{  
		SDL_DestroyWindow(m_Window); 
		ENGINE_CORE_WARN("Shutting down SDL_Window"); 
		SDL_Delay(10); 
		SDL_Quit();
	}
}
