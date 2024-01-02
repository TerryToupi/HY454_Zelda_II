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

		virtual void EventPolling() override;
		virtual void* GetKeyboardState() const override;
		virtual void SetEventCallBack(const EventCallBackFunciton& EventCallBack) override { m_EventCallBack = EventCallBack; }

		virtual void* GetNativeWindow() const override { return m_Window; }
		virtual void* GetNativeRenderer() const override { return m_Renderer; } 
		virtual void ResizeWindow(int w, int h) const override { SDL_RenderSetLogicalSize(m_Renderer, w, h); }

	private:
		void Init(const WindowConfig& config); 
		void ShutDown();
	private: 
		SDL_Window* m_Window; 
		SDL_Renderer* m_Renderer;
		WindowData m_Data;
		EventCallBackFunciton m_EventCallBack;
	};
}


