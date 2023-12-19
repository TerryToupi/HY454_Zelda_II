#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Math/Math.h>

namespace Engine
{
	class MotionQuantizer
	{
	public:
		using Mover = std::function<void(const Rect& r, int* dx, int* dy)>;

		MotionQuantizer() = default; 
		MotionQuantizer(const MotionQuantizer& other) = default;
		
		MotionQuantizer& SetUsed(bool val); 
		MotionQuantizer& SetRange(int h, int v);  
		MotionQuantizer& SetMover(const Mover& f); 
		
		void Move(const Rect& r, int* dx, int* dy);

	protected:
		int		m_Hmax = 0, m_Vmax = 0;
		Mover	m_Mover; 
		bool	m_Used = false;
	};
}
