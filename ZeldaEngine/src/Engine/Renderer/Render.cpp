#include "Render.h"
#include <Engine/Application/Application.h> 

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
		this->m_Interbuff->Generate(24 * 16, 17 * 16); 
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
		s_Instance->m_Threads.push_back(std::thread{ Renderer::DisplaySceneTilesThread, s_Instance->m_ActiveScene }); 
	}
	
	void Renderer::DisplaySceneTilesThread(Reference<Scene> scene)
	{ 
		int width = (int)s_Instance->InterBufferInstance().GetWidth();   
		int height = (int)s_Instance->InterBufferInstance().GetHeight();
		scene->GetTiles()->Display(
			s_Instance->InterBufferInstance(),
			{ 0, 0, width, height }
		);
	}

	void Renderer::EndScene()
	{  
		if (!s_Instance->m_Threads.empty())
		{
			for (auto& thread : s_Instance->m_Threads)
			{
				if (thread.joinable())
					thread.join();
			}
			s_Instance->m_Threads.clear();
		}  

		s_Instance->m_ActiveScene.reset();
	}  

	void Renderer::BufferFlip()
	{  
		auto& ib = s_Instance->InterBufferInstance(); 
		auto renderer = static_cast<SDL_Renderer*>(Application::Instance().GetWindow().GetNativeRenderer()); 
		SDL_Texture* fb = SDL_CreateTextureFromSurface(renderer, ib.GetSurfice());
		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, fb, NULL, NULL);  
		SDL_RenderPresent(renderer);  
		SDL_DestroyTexture(fb); 
	}
}  

