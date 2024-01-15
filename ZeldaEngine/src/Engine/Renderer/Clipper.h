#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Math/Math.h> 

namespace Engine
{
	class Clipper
	{
	public:
		using View = std::function<const Rect& (void)>;

		Clipper(void) = default;
		Clipper(const Clipper& other) = default; 

		Clipper& SetView(const View& f);
		bool Clip(
			Rect& r,
			const Rect& dpyArea,
			Rect* dpyPos,
			Rect* clippedBox
		)	const;  

	private:
		View m_View;
	};
}

