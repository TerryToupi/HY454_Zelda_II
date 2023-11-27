#include <EngineApp.h>
#include "skoupidi.h" 
#include "Engine/DLLEntryPoint.h"
#include <string>

using namespace Engine;

class Zelda : public Engine::Application
{ 
public: 
	 
	Zelda() 
	{
		pushLayer(new skoupidi());
	} 

	~Zelda() 
	{
		
	} 

}; 

Engine::Application* Engine::CreateApplication() 
{
	Zelda* game = new Zelda; 
	//initializations
	return (Application*)game;
} 

