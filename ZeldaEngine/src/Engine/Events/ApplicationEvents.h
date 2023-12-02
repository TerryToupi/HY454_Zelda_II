#pragma once 
#include <Engine/Events/Event.h>

namespace Engine
{
	class WindowCloseEvent : public Event
	{ 
	public:
		WindowCloseEvent() = default; 

		static EventType GetEventTypeStatic() { return EventType::WindowClose; } 
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return ApplicationEvents; }
	};
}
