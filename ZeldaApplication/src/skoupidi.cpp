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
	m_Tiles->Display(fb, { 640, 480 });
}

void skoupidi::onEvent(Event& e)
{    

}  
