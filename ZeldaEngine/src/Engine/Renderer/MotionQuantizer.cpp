#include "MotionQuantizer.h" 

namespace Engine
{ 
	template<typename T>
	int number_sign(T x)
	{
		return x > 0 ? 1 : x < 0 ? -1 : 0;
	}

	MotionQuantizer& MotionQuantizer::SetUsed(bool val)
	{ 
		m_Used = val; 
		return *this;
	} 

	MotionQuantizer& MotionQuantizer::SetRange(int h, int v)
	{
		m_Hmax = h; 
		m_Vmax = v; 
		return *this;
	} 

	MotionQuantizer& MotionQuantizer::SetMover(const Mover& f)
	{
		m_Mover = f; return *this;
	} 

	void MotionQuantizer::Move(Rect& r, int* dx, int* dy)
	{
		if (!m_Used)
			m_Mover(r, dx, dy); 
		else
		{
			do
			{
				auto sign_x = number_sign(*dx);
				auto sign_y = number_sign(*dy); 
				auto dxFinal = sign_x * std::min(m_Hmax, sign_x * (*dx));
				auto dyFinal = sign_y * std::min(m_Vmax, sign_y * (*dy)); 

				m_Mover(r, &dxFinal, &dyFinal);

				if (!dxFinal)
					*dx = 0;
				else
					*dx -= dxFinal;

				if (!dyFinal)
					*dy = 0;
				else
					*dy -= dyFinal;
			} while (*dx || *dy);
		}
	}
}
