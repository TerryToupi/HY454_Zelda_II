#pragma once 

#include "Engine/Core.h"  
#include "Engine/Logging/log.h"  

#include <filesystem> 

//-------------------------------- don't use these macros outside this file ------------------------------------------------------------
#ifdef ENGINE_PLATFORM_WINDOWS
	#define ENGINE_DEBUG_BREAK		__debugbreak() 
	#define MACRO_TO_STRING(x)		#x	
#endif

#ifdef ENGINE_DEBUG
	#define ENGINE_CORE_ASSERT_IMP(check, msg, ...)		{ if(!(check)) { ENGINE_CORE_ERROR(msg, __VA_ARGS__); ENGINE_DEBUG_BREAK; } }
	#define ENGINE_ASSERT_IMP(check, msg, ...)			{ if(!(check)) { ENGINE_ERROR(msg, __VA_ARGS__); ENGINE_DEBUG_BREAK; } } 
#endif 
//------------------------------------------------------------------------------------------------------------------------------------- 



//------------------------------- THESE ARE THE ASSERT IMPLEMENTATIONS FOR USE OUTSIDE THIS FILE --------------------------------------
#ifdef ENGINE_DEBUG  
	#define ENGINE_ASSERT(check)			ENGINE_ASSERT_IMP(check, "Assertion '{0}' failed at {1}:{2}", MACRO_TO_STRING(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	#define ENGINE_CORE_ASSERT(check)		ENGINE_CORE_ASSERT_IMP(check, "Assertion '{0}' failed at {1}:{2}", MACRO_TO_STRING(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
#else  
	#define ENGINE_ASSERT(check)
	#define ENGINE_CORE_ASSERT(check)
#endif
//------------------------------- THESE ARE THE ASSERT IMPLEMENTATIONS FOR USE OUTSIDE THIS FILE --------------------------------------
