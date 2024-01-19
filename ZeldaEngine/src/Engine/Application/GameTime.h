#pragma once 

#include<Engine/Application/Core.h>

#include<chrono>

namespace Engine 
{ 
	typedef uint64_t Time;  

	class SystemClock final
	{ 
	public:  
		static auto Get(void) -> SystemClock& 
							{ return s_Instance; }  
		
		Time milli_sec(void) const;
		Time micro_sec(void) const;
		Time nano_sec(void) const;  
		Time sec(void) const;

		Time GetTime(void) const; 
		static Time GetDeltaTime(void); 
		static void SetDeltaTime(Time t);

	private:
		std::chrono::high_resolution_clock m_Clock;  
		Time delta;

	private:
		static SystemClock s_Instance;
	}; 
}
