// Engine API/HOOK
#include <Engine.h>
#include <Engine/EngineHook.h>

// Game application include 
#include "Layers/Layer0.h"   
#include "Layers/Layer1.h"

// STD::LIB include
#include <string>

using namespace Engine;

class Zelda : public Engine::Application
{ 
public: 
	 
	Zelda(const Engine::ApplicationConfig& config)
		: Engine::Application(config)
	{
		pushLayer(new Layer0());
		pushLayer(new Layer1());
	} 

	~Zelda() 
	{
		
	} 

}; 

Engine::Application* Engine::CreateApplication() 
{ 
	// config
	Engine::ApplicationConfig appConfig;
	appConfig.appName	 = "Zelda_II";
	appConfig.WorkingDir = "Dialou mana";
	Zelda* game = new Zelda(appConfig); 
	//initializations
	return (Application*)game;
} 

