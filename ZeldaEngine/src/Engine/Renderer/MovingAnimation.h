#pragma once 

#include<Engine/Application/Core.h> 
#include<Engine/Renderer/Animation.h>

namespace Engine
{
	class MovingAnimation : public Animation
	{
	public: 
		using Me = MovingAnimation; 

	public: 
		int			GetDx(void) const			{ return m_Dx; } 
		Me&			SetDx(int32_t _Dx)			{ m_Dx = _Dx; return *this; }
		int			GetDy(void) const			{ return m_Dy; } 
		Me&			SetDy(int32_t _Dy)			{ m_Dy = _Dy; return *this; }
		unsigned	GetDelay(void) const		{ return m_Delay; } 
		Me&			SetDelay(unsigned _Delay)	{ m_Delay = _Delay; return *this; } 
		unsigned	GetReps(void) const			{ return m_Reps; } 
		Me&			SetReps(unsigned _Reps)		{ m_Reps = _Reps; return *this; } 
		bool		IsForEver(void) const		{ return !m_Reps; } 
		Me&			SetForEver(void)			{ m_Reps = 0; return *this; } 
		Animation*	Clone(void) const override
			{ return new MovingAnimation(m_Id, m_Reps, m_Dx, m_Dy, m_Delay); } 
		
		MovingAnimation(
			const std::string& _id, unsigned _reps, int _dx, int _dy, unsigned _delay
		) : Animation(_id), m_Reps(_reps), m_Dx(_dx), m_Dy(_dy), m_Delay(_delay) { }

	private: 
		int32_t m_Dx = 0; 
		int32_t m_Dy = 0; 
		unsigned m_Delay = 0; 
		unsigned m_Reps = 1;
	};  




	class FrameRangeAnimation : public MovingAnimation
	{
	public:
		using Me = FrameRangeAnimation; 

	public:  
		FrameRangeAnimation(
			const std::string& _id,
			uint32_t s, uint32_t e,
			uint32_t r, uint32_t dx, uint32_t dy, uint32_t d
		) : m_Start(s), m_End(e), MovingAnimation(_id, r, dx, dy, d) { }

		uint32_t	GetStartFrame(void) const { return m_Start; } 
		Me&			SetStartFrame(uint32_t v) { m_Start = v; return *(this); } 
		uint32_t	GetEndFrame(void) const	  { return m_End; } 
		Me&			SetEndFrame(uint32_t v)	  { m_End = v; return *(this); } 
		Animation*	Clone(void) const override
		{
			return new FrameRangeAnimation(
				m_Id, m_Start, m_End, GetReps(), GetDx(), GetDy(), GetDelay()
			);
		}

	protected: 
		uint32_t m_Start = 0;
		uint32_t m_End	 = 0;
	};  




	class FrameListAnimation : public MovingAnimation
	{
	public: 
		using Frames = std::vector<uint32_t>;  
		using Me	 = FrameListAnimation;

	public:  
		FrameListAnimation(
			const std::string& _id,
			const Frames& _frames, 
			uint32_t r, uint32_t dx, uint32_t dy, uint32_t d 
		) : m_Frames(_frames), MovingAnimation(_id, r, dx, dy, d) { }

		const Frames& GetFrames(void) const { return m_Frames; } 
		Me&			  SetFrames(const Frames& f) { m_Frames = f; return *(this); } 
		Animation* Clone(void) const override
		{
			return new FrameListAnimation(
				m_Id, m_Frames, GetReps(), GetDx(), GetDy(), GetDelay()
			);
		}

	protected: 
		Frames	m_Frames;
	};
}
