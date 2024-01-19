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
		m_quantizer.SetMover(m_mover = move);
	}

	Rect SpriteClass::GetBox(void)
	{ 
		return { m_x, m_y, m_frameBox.w, m_frameBox.h };
	} 

	SpriteClass& SpriteClass::Move(int dx, int dy)
	{   
		if (m_directMotion)
		{  
			m_x += dx; 
			m_y += dy;
		} 
		else
		{
			Rect r = GetBox();
			m_quantizer.Move(r, &dx, &dy);
			m_gravity.Check(r);
		} 

		return *this;
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
		m_boundingArea = area;
	}

	void SpriteClass::SetColiderBox(unsigned _w, unsigned _h)
	{
		ENGINE_CORE_ASSERT(m_currFilm == nullptr); 
		m_frameBox.w = _w;
		m_frameBox.h = _h;
	}

	void SpriteClass::SetMotionQuantizerUse(bool v)
	{ 
		m_quantizer.SetUsed(v);
	}

	void SpriteClass::SetQuanntizerVertHorz(int v, int h)
	{ 
		m_quantizer.SetRange(h, v);
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

	bool SpriteClass::CollisionCheck(const Sprite s) 
	{ 
		SetBoundingArea(new BoundingBox(m_x, m_y, m_frameBox.w + m_x, m_frameBox.h + m_y));  
		s->SetBoundingArea(new BoundingBox(s->m_x, s->m_y, s->m_frameBox.w + s->m_x, s->m_frameBox.h + s->m_y));
		bool outcome = m_boundingArea->Intersects(*s->GetBoundingArea()); 
		m_boundingArea->Destroy(); 
		s->m_boundingArea->Destroy(); 
		return outcome;
	}

	GravityHandler& SpriteClass::GetGravityHandler(void)
	{
		return m_gravity;
	}

	SpriteClass& SpriteClass::SetHasDirectMotion(bool v)
	{ 
		m_directMotion = v;
		return *this;
	}

	bool SpriteClass::GetHasDirectMotion(void) const
	{
		return m_directMotion;
	}

	std::string& SpriteClass::GetHashName()
	{ 
		return m_hashName;
	}

	void SpriteClass::Display(Bitmap& dest, const Rect& dpyArea, const Clipper& clipper)
	{ 
		Rect clippedBox; 
		Rect dpyPos; 
		Rect r = GetBox();

		if (clipper.Clip(r, dpyArea, &dpyPos, &clippedBox))
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

