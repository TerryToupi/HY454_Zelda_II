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

	class WindowResizeEvent : public Event
	{ 
	public:
		WindowResizeEvent(uint64_t width, uint64_t height)  
			:	m_Width(width), m_Height(height)
		{ 
			ENGINE_CORE_TRACE("Window Resize dimension: W:{0}, H:{1}", m_Width, m_Height);
		} 

		uint64_t GetWidth() { return m_Width; }
		uint64_t GetHeight() { return m_Height; }

		static EventType GetEventTypeStatic() { return EventType::WindowResize; } 
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return ApplicationEvents; } 
	private:
		uint64_t m_Width;
		uint64_t m_Height;
	};
}
