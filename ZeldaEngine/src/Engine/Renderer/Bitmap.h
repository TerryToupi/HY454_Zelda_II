#pragma once   

#include<Engine/Application/Core.h> 
#include<Engine/Math/Math.h> 

#include<SDL.h>

#include<string>

namespace Engine 
{ 
	class Bitmap
	{  
	public:      
		using Pixel8 = Uint8;
		using Pixel16 = Uint16;
		using Pixel32 = Uint32; 

		using PixelMemory = void*;
		using BitmapAccessFunctor = std::function<void(Bitmap& ,PixelMemory)>;

	public:
		Bitmap();
		Bitmap(const std::string path, uint64_t Width, uint64_t Height);
		Bitmap(const Bitmap& other) = default; 
		virtual ~Bitmap(); 
	
		void LoadBMP(std::string path);  

		void Generate(uint32_t width, uint32_t height);

		void SetWidth(uint64_t Width)	{ m_Width = Width; }
		void SetHeight(uint64_t Height) { m_Height = Height; } 

		uint64_t GetWidth() { return m_Width; }
		uint64_t GetHeight() { return m_Height; }  
		void Reset();

		static void BitmapAccessPixels(Bitmap& bmp, const BitmapAccessFunctor& f);
		static Uint32 GetPixel32(Bitmap& bmp, PixelMemory mem);

		static void Blit(Bitmap& src, const Rect* from,
						  Bitmap& dest, Rect* to);  

		static void ScaledBlit(Bitmap& src, const Rect* from,  
								Bitmap& dest, Rect* to);   

		static void PresentOnDisplay(Bitmap& src); 

	private:
		SDL_Surface* m_Surface; 

		uint64_t m_Width; 
		uint64_t m_Height;   
	};
}
