#include "skoupidi.h"

#define THE_GREATES_MPAMPIS "Mpampis"

using namespace Engine;

skoupidi::skoupidi()
	: Layer("skoupidi")
{ 
}

void skoupidi::onStart()
{
	m_Scene = MakeReference<Scene>();
	m_Tiles = MakeReference<TileLayer>();
	m_Tiles->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
}

void skoupidi::onDelete()
{ 
	
}

void skoupidi::onUpdate()
{
	auto& fb = Renderer::FrameBufferInstance().GetBackBuffer();

	Rect disp{ 0, 0, fb.GetWidth(), fb.GetHeight() };
	m_Tiles->Display(fb, disp);
}

void skoupidi::onEvent(Event& e)
{    
	EventDispatcher dispatcher(e); 
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(skoupidi::function));
} 

bool skoupidi::function(KeyTapEvent& e) 
{  
	if (e.GetKey() == InputKey::a)
	{
		ENGINE_TRACE("O MALAKAS PATISE A, GAMW TO SPITI TOU");
	} 
	return true;
}


