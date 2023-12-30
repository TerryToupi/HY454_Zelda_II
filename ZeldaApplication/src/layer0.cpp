#include "layer0.h"

#define THE_GREATES_MPAMPIS "Mpampis"

using namespace Engine;

layer0::layer0()
	: Layer("layer0")
{ 
}

void layer0::onStart()
{
	m_Scene = MakeReference<Scene>(0);
	m_Scene->GetTiles().LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
}

void layer0::onDelete()
{ 
	
}

void layer0::onUpdate()
{
	Renderer::BeginScene(m_Scene); 
	Renderer::DisplaySceneTiles();
	Renderer::EndScene();
}

void layer0::onEvent(Event& e)
{    
	EventDispatcher dispatcher(e); 
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(layer0::mover));
	dispatcher.Dispatch<KeyRepeatEvent>(APP_EVENT_FUNTION(layer0::mover));
} 

bool layer0::mover(KeyTapEvent& e) 
{   
	float SPEED = 4; 

	if (e.GetKey() == InputKey::a)
	{ 
		wdx = -SPEED; 
		wdy = 0;
	}  
	else if (e.GetKey() == InputKey::w)
	{ 
		wdy = -SPEED;   
		wdx = 0;
	}
	else if (e.GetKey() == InputKey::d)
	{ 
		wdx = +SPEED;
		wdy = 0;
	}
	else if (e.GetKey() == InputKey::s)
	{ 
		wdy = +SPEED; 
		wdx = 0;
	}  

	m_Scene->GetTiles().Scroll(wdx, wdy);
	return true;
}

bool layer0::mover(KeyRepeatEvent& e)
{
	float SPEED = 4;

	if (e.GetKey() == InputKey::a)
	{
		wdx = -SPEED;
		wdy = 0;
	}
	else if (e.GetKey() == InputKey::w)
	{
		wdy = -SPEED;
		wdx = 0;
	}
	else if (e.GetKey() == InputKey::d)
	{
		wdx = +SPEED;
		wdy = 0;
	}
	else if (e.GetKey() == InputKey::s)
	{
		wdy = +SPEED;
		wdx = 0;
	}

	m_Scene->GetTiles().Scroll(wdx, wdy);
	return true;
}


