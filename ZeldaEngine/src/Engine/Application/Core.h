#pragma once 

#include <memory>

namespace Engine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args> 
	constexpr Scope<T> MakeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	} 

	template<typename T>
	using Reference = std::shared_ptr<T>;
	template<typename T, typename ...Args> 
	constexpr Reference<T> MakeReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}  

	typedef unsigned char byte;
}   

#define BIT_SHIFT(x)			(1 << x)
#define EVENT_FUNCTION_BIND(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define APP_EVENT_FUNTION(fn) [this](auto&&... args) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <Engine/Logging/Log.h>
#include <Engine/Assert/Assert.h> 
