#include "Sprite.h" 

#include <Engine/Scene/Scene.h> 
#include <Engine/Scene/SpriteData.h> 

namespace Engine
{
	Sprite::Sprite(std::string name, Scene* scene) 
		:	m_HashName(name),	m_Scene(scene)
	{ 
	}

	void Sprite::SetMover(const Mover& move)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		spData->mover = move; 
		spData->quantizer.SetMover(spData->mover); 
		return;
	}

	const Rect Sprite::GetBox(void) const
	{  
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);

		Rect r;
		r.x = spData->x; 
		r.y = spData->y; 
		r.w = spData->frameBox.w;
		r.h = spData->frameBox.h;
		return r;
	}

	Sprite& Sprite::Move(int dx, int dy)
	{  
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		
		if (spData->directMotion)
		{
			spData->x += dx; 
			spData->y += dy;
		} 
		else
		{
			spData->quantizer.Move(GetBox(), &dx, &dy);  
			spData->gravity.Check(GetBox());
		}

		return *this;
	}

	void Sprite::SetPos(int _x, int _y)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 

		spData->x = _x;
		spData->y = _y;  
		return;
	} 

	void Sprite::SetZorder(unsigned z)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		spData->zorder = z; 
		return;
	}

	unsigned Sprite::GetZorder(void)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		return spData->zorder;
	}

	void Sprite::SetFrame(byte i)
	{  
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName); 
		
		if (i != spData->frameNo)
		{ 
			//TODO FILM
		}
	}

	byte Sprite::GetFrame(void) const
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);  
		return spData->frameNo;
	} 

	const std::string& Sprite::GetTypeId()
	{
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->typeID;
	}

	void Sprite::SetVisibility(bool v)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		spData->isVisible = v; 
		return;
	}

	bool Sprite::IsVisible(void) const
	{
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->isVisible;
	}

	// TODO
	bool Sprite::CollisionCheck(const Sprite* s) const
	{
		return false;
	}

	GravityHandler& Sprite::GetGravityHandler(void)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->gravity; 
	}

	Sprite& Sprite::SetHasDirectMotion(bool v)
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		spData->directMotion = v; 
		return *this;
	}

	bool Sprite::GetHasDirectMotion(void) const
	{
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);   
		return spData->directMotion;
	}

	void Sprite::Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper) const
	{ 
		auto* spData = &m_Scene->m_SpritesMap.at(m_HashName);    

		Rect clippedBox; 
		Rect dpyPos;

		if (clipper.Clip(GetBox(), dpyArea, &dpyPos, &clippedBox))
		{
			Rect clippedFrame
			{
				spData->frameBox.x + clippedBox.x,
				spData->frameBox.y + clippedBox.y, 
				clippedBox.w, 
				clippedBox.h
			}; 

			Bitmap::Blit( 
				// TODO INSERT FILM HERE
				Bitmap(), 
				&clippedFrame, 
				dest, 
				&dpyPos
			);
		}
	}
}

