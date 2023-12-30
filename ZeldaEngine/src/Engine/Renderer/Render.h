#pragma once 

#include <Engine/Math/Math.h> 
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Renderer/Framebuffer.h>  
#include <Engine/Scene/Scene.h>

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

		static FrameBuffer&			FrameBufferInstance(); 
		static Scope<FrameBuffer>	CreateFrameBuffer(const uint64_t Width, const uint64_t Height); 
		static Bitmap&				InterBufferInstance(); 
		static Scope<Bitmap>		CreateInterBuffer();
		static void					ResizeFrameBuffer(const uint64_t width, const uint64_t height);  

		static void BeginScene(Reference<Scene> scene); 
		static void DisplaySceneTiles();
		static void	EndScene(); 

		static void BufferFlip();

	private:  
		RendererConfig		m_Config;
		Scope<FrameBuffer>	m_Framebuff;
		Scope<Bitmap>		m_Interbuff; 
		Reference<Scene>	m_ActiveScene;

	private: 
		static Renderer* s_Instance;
	};
}
