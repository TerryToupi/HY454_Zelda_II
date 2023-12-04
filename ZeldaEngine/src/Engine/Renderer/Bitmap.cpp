#include "Bitmap.h"
#include <Engine/Application/Application.h>

namespace Engine
{
	Bitmap::Bitmap()
		: m_Width(0), m_Height(0), m_Surface(nullptr)
	{
	} 

	Bitmap::Bitmap(const std::string path, uint64_t Width, uint64_t Height)
		:	m_Width(Width), m_Height(Height)
	{
		Load(path);
	} 
	
	Bitmap::~Bitmap()
	{  
		clear();
	} 
	
	void Bitmap::clear() 
	{
		SDL_FreeSurface(m_Surface);
	} 
	
	void Bitmap::Blit(Bitmap src, const Rect* from, Bitmap dest, Rect* to)
	{
		auto* window = static_cast<SDL_Window*>(Application::Instance().GetWindow().GetNativeWindow());
		SDL_BlitSurface(src.m_Surface, from, dest.m_Surface, to);   
		SDL_UpdateWindowSurface(window);
	}

	void Bitmap::ScaledBlit(Bitmap src, const Rect* from, Bitmap dest, Rect* to)
	{ 
		auto* window = static_cast<SDL_Window*>(Application::Instance().GetWindow().GetNativeWindow());
		SDL_BlitScaled(src.m_Surface, from, dest.m_Surface, to);   
		SDL_UpdateWindowSurface(window);
	}


	void Bitmap::Load(std::string path)
	{ 
		m_Surface = SDL_LoadBMP(path.c_str()); 
	}
}