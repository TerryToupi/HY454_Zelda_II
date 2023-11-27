#pragma once 

#include	"Engine/Core.h"

#pragma warning(push, 0)
#include	"spdlog/spdlog.h"
#include	"spdlog/fmt/ostr.h" 
#pragma warning(pop)

#include	<memory>

namespace Engine 
{
	class Log
	{ 
	public:
		static void Init(); 
		static std::shared_ptr<spdlog::logger>& GetCoreLogger()		{ return s_CoreLogger;  }
		static std::shared_ptr<spdlog::logger>& GetClientLogger()	{ return s_ClientLogger;  } 

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} 

#ifdef ENGINE_DEBUG
	#define ENGINE_CORE_TRACE(...)		::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define ENGINE_CORE_INFO(...)		::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define ENGINE_CORE_WARN(...)		::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define ENGINE_CORE_ERROR(...)		::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)

	#define ENGINE_TRACE(...)		::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define ENGINE_INFO(...)		::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
	#define ENGINE_WARN(...)		::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define ENGINE_ERROR(...)		::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#endif 

#ifdef ENGINE_RELEASE  
	#define ENGINE_CORE_TRACE(...)		
	#define ENGINE_CORE_INFO(...)		
	#define ENGINE_CORE_WARN(...)		
	#define ENGINE_CORE_ERROR(...)		

	#define ENGINE_TRACE(...)		
	#define ENGINE_INFO(...)		
	#define ENGINE_WARN(...)		
	#define ENGINE_ERROR(...)		
#endif
