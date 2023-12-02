#include "Window.h"
#include "WindowAPI.h"


namespace Engine 
{
	Scope<Window> Engine::Window::Create(const WindowConfig& config)
	{ 
		ENGINE_CORE_WARN("Making Window, only available paltform configuration: Windows 7/10/11");
		return MakeScope<WindowAPI>(config);
	}
}
