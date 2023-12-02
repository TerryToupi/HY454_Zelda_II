#pragma once 

#include <Engine/Application/Core.h> 
#include <functional>

#define EVENT_FUNCTION_BIND(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Engine 
{
	class Event
	{ 
	public: 
		virtual ~Event() = default;

		bool Handled = false;
		// return type = 0;
		// return category flags =0; 

		bool IsInCategory();

	private:
	};
}
