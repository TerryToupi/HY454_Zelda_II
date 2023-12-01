#include "Window.h"
#include "WindowAPI.h"


namespace Engine 
{
	Scope<Window> Engine::Window::create(const WindowConfig& config)
	{ 
		ENGINE_CORE_WARN("Making Window, only available paltform: Windows");
		return MakeScope<WindowAPI>(config);
	}
}
