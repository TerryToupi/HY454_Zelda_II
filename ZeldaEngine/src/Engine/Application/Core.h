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
	constexpr Scope<T> MakeReference(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
} 

#include <Engine/Logging/Log.h>
#include <Engine/Assert/Assert.h>
