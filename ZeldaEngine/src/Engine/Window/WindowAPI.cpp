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

	void WindowAPI::Init(const WindowConfig& config)
	{ 
		m_Data.Height = config.Height;
		m_Data.Width = config.Width;
		m_Data.TitleBar = config.WindowName;  

		int status = SDL_Init(SDL_INIT_EVERYTHING);
		ENGINE_CORE_ASSERT(status); 
	
		SDL_CreateWindowAndRenderer(m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL, &m_Window, &m_Renderer); 
		SDL_SetWindowTitle(m_Window, m_Data.TitleBar.c_str());
	}

	void WindowAPI::ShutDown()
	{  
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		SDL_Quit();
	}
}
