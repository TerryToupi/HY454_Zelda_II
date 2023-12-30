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
		this->m_Interbuff = this->CreateInterBuffer(); 
		this->m_Interbuff->Generate(m_Config.fb_width, m_Config.fb_height); 
		this->m_ActiveScene = nullptr;
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

	FrameBuffer& Renderer::FrameBufferInstance()
	{
		return *s_Instance->m_Framebuff;
	}

	Scope<FrameBuffer> Renderer::CreateFrameBuffer(const uint64_t Width, const uint64_t Height)
	{	
		return MakeScope<FrameBuffer>(Width, Height);
	}

	Bitmap& Renderer::InterBufferInstance()
	{ 
		return *s_Instance->m_Interbuff;
	}

	Scope<Bitmap> Renderer::CreateInterBuffer()
	{
		return MakeScope<Bitmap>();
	}

	void Renderer::ResizeFrameBuffer(const uint64_t width, const uint64_t height)
	{ 
		Renderer::s_Instance->FrameBufferInstance().resize(width, height); 
	} 

	void Renderer::BeginScene(Reference<Scene> scene)
	{ 
		s_Instance->m_ActiveScene = scene;
	}

	void Renderer::DisplaySceneTiles()
	{   
		s_Instance->m_ActiveScene->GetTiles().Display( 
			s_Instance->InterBufferInstance(),
			{ 0, 0, (int)s_Instance->InterBufferInstance().GetWidth(), (int)s_Instance->InterBufferInstance().GetHeight() }
		);
	}

	void Renderer::EndScene()
	{ 
		s_Instance->m_ActiveScene.reset();
	} 

	void Renderer::BufferFlip()
	{ 
		auto& fb = s_Instance->m_Framebuff->GetBackBuffer();  
		auto& ib = s_Instance->InterBufferInstance();

		Bitmap::Blit(ib, NULL, fb, NULL);
	}
}
