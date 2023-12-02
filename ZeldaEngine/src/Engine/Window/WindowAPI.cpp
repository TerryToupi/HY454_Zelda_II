#include "WindowAPI.h"

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
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: 
				ENGINE_CORE_TRACE("Event SDL_QUIT traced");
				Event e;
				m_EventCallBack(e);

			defaut: 
				break;
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
		
		m_Window = SDL_CreateWindow(
			m_Data.TitleBar.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_Data.Width, m_Data.Height,
			0
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
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		SDL_Quit();
	}
}
