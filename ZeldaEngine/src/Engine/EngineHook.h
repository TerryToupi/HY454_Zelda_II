#pragma once 
#include <Engine/Application/Core.h>
#include <Engine/Application/Application.h>

#include <string.h> 
#include <SDL.h>

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

#undef main

int main(int argc, char** argv)
{ 
	Engine::Log::Init();  

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif
