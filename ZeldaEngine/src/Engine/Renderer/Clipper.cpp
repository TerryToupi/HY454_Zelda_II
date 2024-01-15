#include "Clipper.h"

namespace Engine
{ 
	template<class T> 
	bool clip_rect(
		T x, T y, T w, T h,
		T wx, T wy, T ww, T wh,
		T* cx, T* cy, T* cw, T* ch
	)
	{
		*cw = T(std::min(wx + ww, x + w)) - (*cx = T(std::max(wx, x)));
		*ch = T(std::min(wy + wh, y + h)) - (*cy = T(std::max(wy, y)));
		return *cw > 0 && *ch > 0;
	} 

	bool clip_rect_func(Rect& r, const Rect& area, Rect* result) {
		return clip_rect(
			r.x,
			r.y,
			r.w,
			r.h,
			area.x,
			area.y,
			area.w,
			area.h,
			&result->x,
			&result->y,
			&result->w,
			&result->h
		);
	} 

	Clipper& Clipper::SetView(const View& f)
	{ 
		m_View = f; 
		return *this;
	} 

	bool Clipper::Clip(Rect& r, const Rect& dpyArea, Rect* dpyPos, Rect* clippedBox) const
	{ 
		Rect visibleArea;
		if (!clip_rect_func(r, m_View(), &visibleArea))
		{
			clippedBox->w = 0;
			clippedBox->h = 0;  

			return false;
		}
		else
		{
			clippedBox->x = r.x - visibleArea.x;
			clippedBox->y = r.y - visibleArea.y; 

			clippedBox->h = visibleArea.h;
			clippedBox->w = visibleArea.w;
			
			dpyPos->x = dpyArea.x + (visibleArea.x - m_View().x);
			dpyPos->y = dpyArea.y + (visibleArea.y - m_View().y);  

			dpyPos->w = visibleArea.w;
			dpyPos->h = visibleArea.h; 

			return true;
		}
	}
}
