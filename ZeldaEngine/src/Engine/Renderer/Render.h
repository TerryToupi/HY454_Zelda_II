#pragma once 

#include <Engine/Math/Math.h> 
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Scene/Scene.h>  

#include <thread> 

namespace Engine
{ 
	struct RendererConfig
	{
		uint64_t fb_width;
		uint64_t fb_height;
	};
	
	class Renderer
	{ 
	public:
		Renderer(const RendererConfig& config);  
		Renderer(const Renderer& other) = delete;
		~Renderer() = default; 

		static Renderer& Instatnce();      
		static void Init(const RendererConfig& config); 
		static void Shutdown();

		static Bitmap&				InterBufferInstance(); 
		static Scope<Bitmap>		CreateInterBuffer();

		static void BeginScene(Reference<Scene> scene); 
		static void DisplaySceneTiles();
		static void	EndScene(); 

		static void BufferFlip();  

	protected:
		static void DisplaySceneTilesThread(Reference<Scene> scene); 

	private:  
		RendererConfig			 m_Config;
		Scope<Bitmap>			 m_Interbuff; 
		Reference<Scene>		 m_ActiveScene; 
		std::vector<std::thread> m_Threads;
	private: 
		static Renderer* s_Instance;
	};
}
