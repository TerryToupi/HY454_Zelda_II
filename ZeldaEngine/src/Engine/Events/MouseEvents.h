#pragma once 

#include <Engine/Events/Event.h>

namespace Engine
{
	class MouseMotionEvent : public Event
	{
	public: 
		MouseMotionEvent(uint32_t posX, uint32_t posY) 
			:	m_PosX(posX), m_PosY(posY)
		{
		} 
		
		uint32_t GetX() { return m_PosX; }
		uint32_t GetY() { return m_PosY; } 

		static EventType GetEventTypeStatic() { return EventType::MousedMoved; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private: 
		uint32_t m_PosX;
		uint32_t m_PosY;
	}; 

	class MouseClickPressEvent : public Event
	{
	public: 
		MouseClickPressEvent(/*key code*/)
		{
		} 

		static EventType GetEventTypeStatic() { return EventType::MouseButtonPressed; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:  
		// key code
	}; 

	class MouseClickReleaseEvent : public Event
	{
	public: 
		MouseClickReleaseEvent(/*key code*/)
		{
		} 

		static EventType GetEventTypeStatic() { return EventType::MouseButtonReleased; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:  
		// key code
	}; 

	class MouseScrollEvent : public Event
	{
	public:  
		// Xoffset positive right - negative left 
		// Yoffset positive away from user - negative toward user

		MouseScrollEvent(int32_t Xoffset, int32_t Yoffset) 
			: m_Xoffset(Xoffset), m_Yoffset(Yoffset)
		{
		} 

		static EventType GetEventTypeStatic() { return EventType::MouseScrolled; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:   
		int32_t m_Xoffset; 
		int32_t m_Yoffset;
	};
}
