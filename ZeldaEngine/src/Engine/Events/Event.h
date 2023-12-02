#pragma once 

#include <Engine/Application/Core.h> 
#include <functional>

namespace Engine 
{ 
	enum class EventType
	{
		None = 0, 
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MousedMoved, MouseScrolled
	}; 

	enum EventCategories
	{
		None = 0,
		ApplicationEvents = BIT_SHIFT(0),
		InputEvents		  = BIT_SHIFT(1), 
		KeyBoardEvents	  = BIT_SHIFT(2),
		MouseEvensts	  = BIT_SHIFT(3),
		MouseButtonEvents = BIT_SHIFT(4)
	};

	class Event
	{ 
	public: 
		virtual ~Event() = default;

		bool handled = false; 
		virtual EventType GetEventType() const = 0; 
		virtual int GetCategoryFlags() const = 0;

		bool IsInCategory(EventCategories cat)
		{
			return GetCategoryFlags() & cat;
		}
	}; 

	class EventDispatcher
	{
	public:  
		EventDispatcher(Event& event)  
			: m_Event(event)
		{
		} 
		
		template<typename T, typename F>
		bool Dispatch(const F& function)
		{
			if (m_Event.GetEventType() == T::GetEventTypeStatic())
			{
				m_Event.handled = m_Event.handled || function(static_cast<T&>(m_Event)); 
				return true;
			} 

			return false;
		}

	private: 
		Event& m_Event;
	};
}
