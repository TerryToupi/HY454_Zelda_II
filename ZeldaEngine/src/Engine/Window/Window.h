#pragma once 

#include <Engine/Application/Core.h>  
#include <Engine/Events/Event.h>
#include <string>

namespace Engine { 
	struct WindowConfig
	{ 
		std::string WindowName;
		uint32_t Width;
		uint32_t Height; 
	
		WindowConfig(const std::string& title = "Zelda II", 
						uint32_t Width = 1024,
						uint32_t Height = 720)
			: WindowName(title), Width(Width), Height(Height)
		{
		}
	};

	class Window
	{ 
	public: 
		using EventCallBackFunciton = std::function<void(Event&)>;

		virtual ~Window() = default;   

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0; 

		virtual void* GetNativeWindow() const = 0; 
		virtual void* GetNativeRenderer() const = 0;  

		virtual void EventPolling() = 0;
		virtual void SetEventCallBack(const EventCallBackFunciton& EventCallBack) = 0;

		static Scope<Window> Create(const WindowConfig& config);
	};
}

