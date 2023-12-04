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
}
