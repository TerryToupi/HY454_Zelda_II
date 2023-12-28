#pragma once   

#include<Engine/Application/Core.h> 
#include<Engine/Math/Math.h> 

#include<SDL.h>

#include<string>

namespace Engine 
{ 
	class FrameBuffer;

	class Bitmap
	{  
	public:  

		Bitmap();
		Bitmap(const std::string path, uint64_t Width, uint64_t Height);
		Bitmap(const Bitmap& other) = default; 
		virtual ~Bitmap(); 
	
		void Load(std::string path);  

		void Generate(uint32_t width, uint32_t height);

		void SetWidth(uint64_t Width)	{ m_Width = Width; }
		void SetHeight(uint64_t Height) { m_Height = Height; } 

		uint64_t GetWidth() { return m_Width; }
		uint64_t GetHeight() { return m_Height; } 

		static void Reset(Bitmap& b);

		static void Blit(Bitmap& src, const Rect* from,
						  Bitmap& dest, Rect* to);  

		static void ScaledBlit(Bitmap& src, const Rect* from, 
								Bitmap& dest, Rect* to); 
	protected: 
		void SetSurfice(SDL_Surface* surface) { m_Surface = surface; }   

	protected:
		friend class FrameBuffer;

	private:
		SDL_Surface* m_Surface; 

		uint64_t m_Width; 
		uint64_t m_Height;   
	};
}
