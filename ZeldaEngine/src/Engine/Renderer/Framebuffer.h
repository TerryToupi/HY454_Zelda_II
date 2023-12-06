#pragma once 

#include<Engine/Renderer/Bitmap.h> 

namespace Engine
{
	class FrameBuffer
	{ 
	public:  
		FrameBuffer(const uint64_t width, const uint64_t height);
	
		void resize(const uint64_t width, const uint64_t height); 

		uint64_t GetHeight() { return m_BackBuffer.GetHeight(); }
		uint64_t GetWidth() { return m_BackBuffer.GetWidth(); }
		
		operator Bitmap() const { return m_BackBuffer; } 

	private:
		void Init(const uint64_t width, const uint64_t height); 

	private:  
		Bitmap m_BackBuffer;
	};
}
