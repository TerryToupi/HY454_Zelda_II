#include "Framebuffer.h"
#include <Engine/Application/Application.h> 

#include<SDL.h>  

namespace Engine
{
	FrameBuffer::FrameBuffer(const uint64_t width, const uint64_t height)
	{ 
		Init(width, height);
	} 
	
	void FrameBuffer::Init(const uint64_t width, const uint64_t height)
	{
		auto* window = static_cast<SDL_Window*>(Application::Instance().GetWindow().GetNativeWindow());
		this->m_BackBuffer.SetHeight(height); 
		this->m_BackBuffer.SetWidth(width);

		this->m_BackBuffer.SetSurfice(SDL_GetWindowSurface(window));
	}

	void FrameBuffer::resize(const uint64_t width, const uint64_t height)
	{ 
		auto* window = static_cast<SDL_Window*>(Application::Instance().GetWindow().GetNativeWindow()); 
		SDL_SetWindowSize(window, width, height); 
		m_BackBuffer.SetHeight(height); 
		m_BackBuffer.SetWidth(width);
	}
}
