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
	ENGINE_CORE_WARN("Engine initialized"); 
	ENGINE_INFO("Application initialized"); 
	ENGINE_CORE_TRACE("Application initialized");  

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("First Instance of a window", 150, 150, 1600, 900, SDL_WINDOW_OPENGL);

	SDL_Delay(5000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	/*auto app = Engine::CreateApplication();
	app->Run();
	delete app;*/
}

#endif
