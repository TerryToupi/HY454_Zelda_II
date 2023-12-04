#pragma once 

#include <Engine/Window/Window.h> 
#include <SDL.h>

namespace Engine
{ 
	struct WindowData
	{
		std::string TitleBar;
		uint32_t Width;
		uint32_t Height; 
		bool VSync; 
	};  

	class WindowAPI : public Window
	{
	public:
		WindowAPI(const WindowConfig& config); 
		virtual ~WindowAPI(); 

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; } 

		virtual void EventPolling();
		virtual void SetEventCallBack(const EventCallBackFunciton& EventCallBack) override { m_EventCallBack = EventCallBack; }

		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WindowConfig& config); 
		void ShutDown();
	private: 
		SDL_Window* m_Window;  
		WindowData m_Data;
		EventCallBackFunciton m_EventCallBack;
	};
}


