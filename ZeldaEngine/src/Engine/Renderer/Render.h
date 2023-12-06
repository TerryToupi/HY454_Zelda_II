#pragma once 

#include <Engine/Math/Math.h> 
#include <Engine/Renderer/Bitmap.h> 
#include <Engine/Renderer/Framebuffer.h> 


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

		static FrameBuffer& GetFrameBuffer();
		static Scope<FrameBuffer> CreateFrameBuffer(const uint64_t Width, const uint64_t Height);
		static void ResizeFrameBuffer(const uint64_t width, const uint64_t height);
	private:  
		RendererConfig m_Config;
		Scope<FrameBuffer> m_Framebuff; 

	private: 
		static Renderer* s_Instance;
	};
}
