#pragma once   

#include<Engine/Application/Core.h>
#include<SDL.h> 

#include<string>

namespace Engine 
{
	class Bitmap
	{  
	public:  
		using Rect = SDL_Rect; 
		using Point = SDL_Point;

		Bitmap();
		Bitmap(const std::string path, uint64_t Width, uint64_t Height);
		Bitmap(const Bitmap& other) = default; 
		~Bitmap(); 
	
		void Load(std::string path);  
		void SetWidth(uint64_t Width)	{ m_Width = Width; }
		void SetHeight(uint64_t Height) { m_Height = Height; } 
		void clear();   

		void SetSurfice(SDL_Surface* surface) { m_Surface = surface; }

		uint64_t GetWidth() { return m_Width; }
		uint64_t GetHeight() { return m_Height; }


		static void Blit(Bitmap src, const Rect* from,
						  Bitmap dest, Rect* to);  

		static void ScaledBlit(Bitmap src, const Rect* from, 
								Bitmap dest, Rect* to);
	private: 
		SDL_Surface* m_Surface; 

		uint64_t m_Width; 
		uint64_t m_Height;
	};
}
