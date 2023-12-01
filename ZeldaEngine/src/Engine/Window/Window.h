#pragma once 

#include <Engine/Application/Core.h>  
#include <string>

namespace Engine { 
	struct WindowConfig
	{ 
		std::string WindowName;
		uint32_t Width;
		uint32_t Height; 
	
		WindowConfig(const std::string& title = "Zelda II", 
						uint32_t Width = 1600,
						uint32_t Height = 900)
			: WindowName(title), Width(Width), Height(Height)
		{
		}
	};

	class Window
	{ 
	public:
		virtual ~Window() = default;   

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0; 

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> create(const WindowConfig& config = WindowConfig());
	};
}

