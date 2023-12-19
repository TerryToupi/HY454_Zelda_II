#include "GameTime.h"

namespace Engine
{
	SystemClock* SystemClock::s_Instance = nullptr; 

	void SystemClock::Init(void)
	{ 
		ENGINE_CORE_ASSERT(!s_Instance); 
		SystemClock::s_Instance = new SystemClock();
	}

	void SystemClock::Shutdown(void)
	{ 
		delete SystemClock::s_Instance;
	}

	uint64_t SystemClock::milli_sec(void) const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::micro_sec(void) const
	{ 
		return std::chrono::duration_cast<std::chrono::microseconds>(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::nano_sec(void) const
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>
			(m_Clock.now().time_since_epoch()).count();
	}

	uint64_t SystemClock::GetTime(void) const
	{
		return SystemClock::milli_sec();
	}
}
