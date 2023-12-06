#pragma once 

#include<Engine/Application/Core.h>

#include<chrono>

namespace Engine 
{ 
	typedef uint64_t Time;  

	class SystemClock 
	{ 
	public:  
		static auto Get(void) -> SystemClock& 
							{ return *s_Instance; }  

		static void Init(void); 
		static void Shutdown(void);
		
		uint64_t milli_sec(void) const;
		uint64_t micro_sec(void) const;
		uint64_t nano_sec(void) const; 

		uint64_t GetTime(void) const;
	public:
		std::chrono::high_resolution_clock m_Clock; 
		static SystemClock* s_Instance;
	}; 
}
