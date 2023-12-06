#pragma once 

#include <Engine/Events/Event.h> 
#include <Engine/Input/KeyBoardCodes.h>

namespace Engine
{
	class KeyTapEvent : public Event
	{ 
	public:
		KeyTapEvent(InputKey key) 
			: m_Key(key)
		{ 
			ENGINE_CORE_TRACE("key tap cuptured: {0}", m_Key);
		} 

		InputKey GetKey() { return m_Key; }

		static EventType GetEventTypeStatic() { return EventType::KeyTyped; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	private: 
		InputKey m_Key;
	}; 

	class KeyRepeatEvent : public Event
	{  
	public:
		KeyRepeatEvent(InputKey key, bool active) 
			: m_Key(key), m_Active(active)
		{
			ENGINE_CORE_TRACE("key press cuptured: {0}", m_Key);
		} 

		InputKey GetKey() { return m_Key; }
	
		static EventType GetEventTypeStatic() { return EventType::KeyPressed; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	private:  
		InputKey m_Key;
		bool m_Active;
	}; 

	class KeyReleaseEvent : public Event
	{ 
	public:
		KeyReleaseEvent(InputKey key) 
			: m_Key(key)
		{
			ENGINE_CORE_TRACE("key release cuptured: {0}", m_Key);
		} 

		InputKey GetKey() { return m_Key; }

		static EventType GetEventTypeStatic() { return EventType::KeyReleased; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); } 

	private:
		InputKey m_Key;
	};
}
