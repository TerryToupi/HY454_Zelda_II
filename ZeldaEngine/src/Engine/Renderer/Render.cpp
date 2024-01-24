#include "Render.h"

namespace Engine
{ 
	Renderer* Renderer::s_Instance = nullptr; 

	Renderer::Renderer(const RendererConfig& config)
	{
		ENGINE_CORE_ASSERT(!s_Instance);  
		this->m_Config = config;
		this->m_Interbuff = this->CreateInterBuffer(); 
		this->m_Interbuff->Generate(21 * 16, 16 * 16); 
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

	Bitmap& Renderer::InterBufferInstance()
	{ 
		return *s_Instance->m_Interbuff;
	}

	Scope<Bitmap> Renderer::CreateInterBuffer()
	{
		return MakeScope<Bitmap>();
	}

	void Renderer::BeginScene(Ref<Scene> scene)
	{ 
		s_Instance->m_ActiveScene = scene;
	}

	void Renderer::DisplaySceneTiles()
	{ 
		Rect viewPort = s_Instance->m_ActiveScene->GetTiles()->GetViewWindow();
		s_Instance->m_ActiveScene->GetTiles()->Display(
			s_Instance->InterBufferInstance(),
			{ 0, 0, viewPort.w, viewPort.h }
		);
	}

	void dispDebugGrid(Bitmap& dest, int x, int y, int w, int h)
	{ 
		Bitmap red; 
		Rect r{ x, y, w, h };
		red.LoadBMP("Assets/DebugAssets/4_tile_red.bmp");
		Bitmap::Blit(red, NULL, dest, &r);
	}
	
	void Renderer::DebugDisplayGrid()
	{ 
		auto& ib = s_Instance->InterBufferInstance(); 
		s_Instance->m_ActiveScene->GetTiles()->DisplayGrid(ib, dispDebugGrid);
	}


	const Clipper MakeTileLayerClipper(TileLayer* layer)
	{
		return Clipper().SetView(
			[layer](void) -> const Rect&
			{ return layer->GetViewWindow(); }
		);
	}

	void Renderer::DisplaySprites()
	{
		auto& ib = s_Instance->InterBufferInstance(); 
		Rect viewPort = s_Instance->m_ActiveScene->GetTiles()->GetViewWindow();

		for (auto sprite : s_Instance->m_ActiveScene->GetSpriteManager().GetDisplayList())
		{
			if (sprite && sprite->GetFilm())
			{ 
				sprite->Display(
					ib,
					{0, 0, viewPort.w, viewPort.h},
					MakeTileLayerClipper(s_Instance->m_ActiveScene->GetTiles().get())
				);
			}
		}
	}

	void Renderer::EndScene()
	{  
		s_Instance->m_ActiveScene.reset();
	}  

	void Renderer::BufferFlip()
	{  
		auto& ib = s_Instance->InterBufferInstance(); 
		Bitmap::PresentOnDisplay(ib);
	}
}  

