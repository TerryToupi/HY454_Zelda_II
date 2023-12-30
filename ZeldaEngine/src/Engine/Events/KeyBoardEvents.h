#pragma once 

#include <Engine/Events/Event.h> 
#include <Engine/Input/KeyBoardCodes.h>

namespace Engine
{ 
	class KeyPressEvent : public Event
	{ 
	public:
		KeyPressEvent(InputKey key) 
			: m_Key(key)
		{ 
		}  

		virtual ~KeyPressEvent() = default;

		InputKey GetKey() { return m_Key; }

		static EventType GetEventTypeStatic() { return EventType::KeyTyped; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }

	private: 
		friend class KeyTapEvent; 
		friend class KeyRepeatEvent;

	private:
		InputKey m_Key;
	}; 

	class KeyTapEvent : public KeyPressEvent 
	{ 
	public:
		KeyTapEvent(InputKey key) 
			: KeyPressEvent(key)
		{ 
			ENGINE_CORE_TRACE("key tap cuptured: {0}", m_Key);
		} 

		InputKey GetKey() { return m_Key; }

		static EventType GetEventTypeStatic() { return EventType::KeyTyped; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	}; 

	class KeyRepeatEvent : public KeyPressEvent 
	{  
	public:
		KeyRepeatEvent(InputKey key, bool active) 
			: KeyPressEvent(key), m_Active(active)
		{
			ENGINE_CORE_TRACE("key press cuptured: {0}", m_Key);
		} 

		InputKey GetKey() { return m_Key; }
	
		static EventType GetEventTypeStatic() { return EventType::KeyPressed; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (KeyBoardEvents | InputEvents); }
	private:  
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
