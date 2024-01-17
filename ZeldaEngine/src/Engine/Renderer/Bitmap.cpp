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

	void Bitmap::BitmapAccessPixels(Bitmap& bmp, const BitmapAccessFunctor& f)
	{ 
		auto result = SDL_LockSurface(bmp.m_Surface); 
		ENGINE_CORE_ASSERT(result == 0);
		
		auto mem = bmp.m_Surface->pixels; 
		auto offset = bmp.m_Surface->pitch;
		auto buffOffset = bmp.m_Surface->pitch / bmp.m_Surface->w;  

		for (auto y = bmp.m_Surface->h; y--;)
		{
			auto buff = mem; 
			for (auto x = bmp.m_Surface->w; x--;)
			{
				f(bmp, buff);  
				buff = (void*)((char*)buff + buffOffset);
			}  

			mem = (void*)((char*)(mem) + offset);
		} 

		SDL_UnlockSurface(bmp.m_Surface);
	}

	Uint32 Bitmap::GetPixel32(Bitmap& bmp, PixelMemory mem)
	{
		Uint8 r, g, b, a;
		SDL_GetRGBA(*(Uint32*)mem, bmp.m_Surface->format, &r, &g, &b, &a); 
		//ENGINE_CORE_TRACE("r:{0}, g:{1}, b:{2}, a:{3}", r, g, b, a); 
		return SDL_MapRGBA(bmp.m_Surface->format, r, g, b, a); 
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
		if (m_Surface == nullptr)
		{ 
			Uint32 rmask, gmask, bmask, amask;
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					rmask = 0xff000000;
					gmask = 0x00ff0000;
					bmask = 0x0000ff00;
					amask = 0x000000ff;
			#else
					rmask = 0x000000ff;
					gmask = 0x0000ff00;
					bmask = 0x00ff0000;
					amask = 0xff000000;
			#endif

			m_Surface = SDL_CreateRGBSurface(
				0, 
				width, 
				height, 
				32, 
				rmask,
				gmask,
				bmask,
				amask
			); 

			SDL_SetSurfaceBlendMode(m_Surface, SDL_BLENDMODE_BLEND); 
			SetHeight(height);
			SetWidth(width);
		}
	}
}