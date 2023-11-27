#pragma once 
#include <Engine/Application/Core.h>
#include <Engine/Application/Application.h>

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{ 
	Engine::Log::Init(); 
	ENGINE_CORE_WARN("Engine initialized"); 
	ENGINE_INFO("Application initialized"); 
	ENGINE_CORE_TRACE("Application initialized");  

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif
