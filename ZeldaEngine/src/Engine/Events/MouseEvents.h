#pragma once 

#include <Engine/Events/Event.h> 
#include <Engine/Input/MouseCodes.h>

namespace Engine
{
	class MouseMotionEvent : public Event
	{
	public: 
		MouseMotionEvent(uint32_t posX, uint32_t posY) 
			:	m_PosX(posX), m_PosY(posY)
		{
			ENGINE_CORE_TRACE("Mouse motion event cupture: x:{0}, y:{1}", m_PosX, m_PosY);
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
		MouseClickPressEvent(MButton button) 
			: m_Button(button)
		{ 
			ENGINE_CORE_TRACE("Mouse Button press cupture: {0}", m_Button);
		} 

		static EventType GetEventTypeStatic() { return EventType::MouseButtonPressed; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:   
		MButton m_Button;
	}; 

	class MouseClickReleaseEvent : public Event
	{
	public: 
		MouseClickReleaseEvent(MButton button) 
			: m_Button(button)
		{
			ENGINE_CORE_TRACE("Mouse Button release cupture: {0}", m_Button);
		} 

		static EventType GetEventTypeStatic() { return EventType::MouseButtonReleased; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:   
		MButton m_Button;
	}; 

	class MouseScrollEvent : public Event
	{
	public:  
		// Xoffset positive right - negative left 
		// Yoffset positive away from user - negative toward user

		MouseScrollEvent(int32_t Xoffset, int32_t Yoffset) 
			: m_Xoffset(Xoffset), m_Yoffset(Yoffset)
		{ 
			ENGINE_CORE_TRACE("Mouse Screll event cupture: x:{0}, y:{1}", m_Xoffset, m_Yoffset);
		}  

		uint32_t GetX() { return m_Xoffset; }
		uint32_t GetY() { return m_Yoffset; } 

		static EventType GetEventTypeStatic() { return EventType::MouseScrolled; }
		virtual	EventType GetEventType() const override { return GetEventTypeStatic(); }
		virtual int GetCategoryFlags() const override { return (MouseEvensts | InputEvents); }
	private:   
		int32_t m_Xoffset; 
		int32_t m_Yoffset;
	};
}
