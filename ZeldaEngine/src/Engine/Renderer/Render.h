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

		static void BeginScene(Ref<Scene> scene); 
		static void DisplaySceneTiles(); 
		static void UpdateSceneAnimators(Time ts); 
		static void DisplaySprites();
		static void	EndScene(); 

		static void BufferFlip();  

	private:  
		RendererConfig			 m_Config;
		Scope<Bitmap>			 m_Interbuff; 
		Ref<Scene>			     m_ActiveScene; 

	private: 
		static Renderer* s_Instance;
	};
}
