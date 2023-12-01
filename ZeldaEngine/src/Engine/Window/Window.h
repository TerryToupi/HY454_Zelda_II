#pragma once 

#include <Engine/Application/Core.h>  
#include <string>
#include <SDL.h>

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
		
	};
}

