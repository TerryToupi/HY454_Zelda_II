#pragma once 

#include <Engine/Events/Event.h>

namespace Engine
{
	class KeyTapEvent : public Event
	{ 
	public:
		KeyTapEvent() = default;

		static EventType GetEventTypeStatic() { return EventType::KeyTyped; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	private:
	}; 

	class KeyRepeatEvent : public Event
	{  
	public:
		KeyRepeatEvent(bool active) 
			: m_Active(active)
		{
		}
	
		static EventType GetEventTypeStatic() { return EventType::KeyPressed; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	private: 
		bool m_Active;
	}; 

	class KeyReleaseEvent : public Event
	{ 
	public:
		KeyReleaseEvent() = default; 

		static EventType GetEventTypeStatic() { return EventType::KeyReleased; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	};
}
