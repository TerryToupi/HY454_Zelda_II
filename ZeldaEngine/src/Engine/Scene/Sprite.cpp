#include "Sprite.h" 

namespace Engine
{
	SpriteClass::SpriteClass(std::string _name, int _x, int _y, AnimationFilm* _film, const std::string& _typeid)
		: m_hashName(_name), m_x(_x), m_y(_y), m_currFilm(_film), m_typeId(_typeid)
	{  
		if (m_currFilm)
			m_frameNo = m_currFilm->GetTotalFrames();
		else
			m_frameNo = 0;

		SetFrame(0);
	} 

	void SpriteClass::SetMover(const Mover& move)
	{ 
		m_quantizer.SetMover(move);
	}

	const Rect SpriteClass::GetBox(void) const
	{
		return { m_x, m_y, m_frameBox.w, m_frameBox.h };
	} 

	void SpriteClass::Move(int dx, int dy)
	{ 
		m_quantizer.Move(GetBox(), &m_x, &m_y);
	}

	void SpriteClass::SetPos(int _x, int _y)
	{ 
		m_x = _x;
		m_y = _y;
	}

	void SpriteClass::SetZorder(unsigned z)
	{ 
		m_zorder = z;
	}

	unsigned SpriteClass::GetZorder(void)
	{
		return m_zorder;
	}

	void SpriteClass::SetFrame(byte i)
	{
		if (i != m_frameNo)
		{
			m_frameBox = m_currFilm->GetFrameBox(m_frameNo = i);
		}
	}

	int SpriteClass::GetPosX() const
	{
		return m_x;
	}

	int SpriteClass::GetPosY() const
	{
		return m_y;
	}

	byte SpriteClass::GetFrame(void) const
	{
		return m_frameNo;
	}

	void SpriteClass::SetFilm(AnimationFilm* film)
	{ 
		m_currFilm = film;
	}

	void SpriteClass::SetBoundingArea(BoundingArea* area)
	{ 
		ENGINE_CORE_ASSERT(!m_boundingArea); 
		m_boundingArea = area;
	}

	const std::string& SpriteClass::GetTypeId()
	{ 
		return m_typeId;
	}

	void SpriteClass::SetVisibility(bool v)
	{ 
		m_isVisible = v;
	}

	bool SpriteClass::IsVisible(void) const
	{
		return m_isVisible;
	}

	std::string& SpriteClass::GetHashName()
	{ 
		return m_hashName;
	}

	void SpriteClass::Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper) const
	{ 
		Rect clippedBox; 
		Rect dpyPos; 


		if (clipper.Clip(GetBox(), dpyArea, &dpyPos, &clippedBox))
		{ 
			Rect clippedFrame
			{
				m_frameBox.x - clippedBox.x,
				m_frameBox.y - clippedBox.y,
				clippedBox.w,
				clippedBox.h
			};

			m_currFilm->DisplayFrame(
				dest,
				clippedFrame,
				dpyPos,
				m_frameNo
			);
		}
	}

}

