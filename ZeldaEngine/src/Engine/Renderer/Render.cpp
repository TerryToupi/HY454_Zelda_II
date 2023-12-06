#include "Render.h"

namespace Engine
{ 
	Renderer* Renderer::s_Instance = nullptr; 

	Renderer::Renderer(const RendererConfig& config)
	{
		ENGINE_CORE_ASSERT(!s_Instance);  
		this->m_Config = config;
		ENGINE_CORE_ASSERT(!this->m_Framebuff);
		this->m_Framebuff = this->CreateFrameBuffer(config.fb_width, config.fb_height);
	} 

	void Renderer::Init(const RendererConfig& config)
	{ 
		ENGINE_CORE_ASSERT(!s_Instance);  
		Renderer::s_Instance = new Renderer(config);
	}

	void Renderer::Shutdown()
	{ 
		delete Renderer::s_Instance;
	}

	Renderer& Renderer::Instatnce()
	{ 
		return *Renderer::s_Instance;
	}

	FrameBuffer& Renderer::GetFrameBuffer()
	{
		return *s_Instance->m_Framebuff;
	}

	Scope<FrameBuffer> Renderer::CreateFrameBuffer(const uint64_t Width, const uint64_t Height)
	{	
		return MakeScope<FrameBuffer>(Width, Height);
	}

	void Renderer::ResizeFrameBuffer(const uint64_t width, const uint64_t height)
	{ 
		Renderer::s_Instance->GetFrameBuffer().resize(width, height); 
	}

}
