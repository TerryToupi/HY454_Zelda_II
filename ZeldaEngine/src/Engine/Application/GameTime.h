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
		
		uint64_t milli_sec(void) const;
		uint64_t micro_sec(void) const;
		uint64_t nano_sec(void) const;  
		uint64_t sec(void) const;

		uint64_t GetTime(void) const;

	public:
		std::chrono::high_resolution_clock m_Clock;  

	private:
		static SystemClock s_Instance;
	}; 
}
