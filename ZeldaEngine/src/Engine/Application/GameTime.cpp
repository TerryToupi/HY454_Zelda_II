#include "GameTime.h"
#include <SDL.h>

namespace Engine
{
	SystemClock SystemClock::s_Instance; 

	uint64_t SystemClock::milli_sec(void) const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::micro_sec(void) const
	{ 
		return std::chrono::duration_cast<std::chrono::microseconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::nano_sec(void) const
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::sec(void) const
	{
		return std::chrono::duration_cast<std::chrono::seconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::GetTime(void) const
	{
		return SystemClock::milli_sec();
	} 

	Time SystemClock::GetDeltaTime(void)
	{
		return s_Instance.delta;
	}

	void SystemClock::SetDeltaTime(Time t)
	{ 
		s_Instance.delta = t;
	}
}
