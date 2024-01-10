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
		LoadBMP(path);
	} 
	
	Bitmap::~Bitmap()
	{
		SDL_FreeSurface(m_Surface); 
	} 

	void Bitmap::Reset()
	{
		SDL_FillRect(m_Surface, NULL, 0x000000);
	}

	void Bitmap::Blit(Bitmap& src, const Rect* from, Bitmap& dest, Rect* to)
	{
		SDL_BlitSurface(src.m_Surface, from, dest.m_Surface, to);    
	}

	void Bitmap::ScaledBlit(Bitmap& src, const Rect* from, Bitmap& dest, Rect* to)
	{ 
		SDL_BlitScaled(src.m_Surface, from, dest.m_Surface, to);   
	}

	void Bitmap::PresentOnDisplay(Bitmap& src)
	{   
		auto renderer = static_cast<SDL_Renderer*>(Application::Instance().GetWindow().GetNativeRenderer());
		SDL_Texture* fb = SDL_CreateTextureFromSurface(renderer, src.m_Surface);

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, fb, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(fb);
	}

	void Bitmap::LoadBMP(std::string path)
	{ 
		m_Surface = SDL_LoadBMP(path.c_str()); 
	} 

	void Bitmap::Generate(uint32_t width, uint32_t height)
	{ 
		m_Surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
		SDL_SetSurfaceBlendMode(m_Surface, SDL_BLENDMODE_BLEND); 
		SetHeight(height);
		SetWidth(width);
	}
}