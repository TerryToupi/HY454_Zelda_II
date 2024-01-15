#include "BoundingArea.h"

namespace Engine
{
	bool BoundingBox::Intersects(const BoundingBox& box) const
	{
		return !( 
				box.w < x		||
				w	  < box.x	|| 
				box.h < y		||
				h	  < box.y 
				);
	} 

	bool BoundingBox::In(unsigned x, unsigned y) const
	{
		return false;
	} 
}
