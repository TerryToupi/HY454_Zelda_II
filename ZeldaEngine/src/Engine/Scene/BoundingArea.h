#pragma once 

#include <Engine/Application/Core.h> 
#include <Engine/Scene/LatelyDestroyable.h>

namespace Engine
{
	class BoundingBox; 

	class BoundingArea : public LatelyDestroyable
	{ 
	public:  
		virtual bool Intersects(const BoundingArea& area) const = 0; 
		virtual bool In(unsigned x, unsigned y) const = 0; 
		virtual BoundingArea* Clone(void) const = 0; 
		virtual ~BoundingArea() {}  

	protected:
		virtual bool Intersects(const BoundingBox& box) const = 0;  

	protected:
		friend class BoundingBox; 
	};

	class BoundingBox : public BoundingArea
	{
	public: 
		BoundingBox(unsigned _x, unsigned _y, unsigned _w, unsigned _h)  
			: x(_x), y(_y), w(_w), h(_h) { }

		virtual bool Intersects(const BoundingBox& box) const;  

		virtual bool In(unsigned x, unsigned y) const;
		virtual bool Intersects(const BoundingArea& area) const
		{
			return area.Intersects(*this);
		};
		virtual BoundingBox* Clone(void) const
		{
			return new BoundingBox(x, y, w, h);
		}

	protected: 
		unsigned x, y, w, h;
	};
}
