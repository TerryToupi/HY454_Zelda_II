#include "layer0.h"

#define THE_GREATES_MPAMPIS "Mpampis"

using namespace Engine;

layer0::layer0()
	: Layer("layer0")
{ 
}

void layer0::onStart()
{
	m_Scene = MakeReference<Scene>();
	m_Tiles = MakeReference<TileLayer>(0);
	m_Tiles->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
}

void layer0::onDelete()
{ 
	
}

void layer0::onUpdate()
{
	auto& fb = Renderer::FrameBufferInstance().GetBackBuffer();
	Rect disp{ 0, 0, fb.GetWidth(), fb.GetHeight() };
	m_Tiles->Display(fb, disp);
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
		if (m_Tiles->CanScrollHoriz(-SPEED))
		{
			wdx = -SPEED; 
			wdy = 0;
		}
	}  
	else if (e.GetKey() == InputKey::w)
	{ 
		if (m_Tiles->CanScrollVert(+SPEED))
		{
			wdy = -SPEED;   
			wdx = 0;
		}
	}
	else if (e.GetKey() == InputKey::d)
	{ 
		if (m_Tiles->CanScrollHoriz(+SPEED))
		{
			wdx = +SPEED;
			wdy = 0;
		}
	}
	else if (e.GetKey() == InputKey::s)
	{ 
		if (m_Tiles->CanScrollVert(-SPEED))
		{
			wdy = +SPEED; 
			wdx = 0;
		}
	}  

	m_Tiles->Scroll(wdx, wdy);
	return true;
}

bool layer0::mover(KeyRepeatEvent& e)
{
	float SPEED = 4;

	if (e.GetKey() == InputKey::a)
	{
		if (m_Tiles->CanScrollHoriz(-SPEED))
		{
			wdx = -SPEED;
			wdy = 0;
		}
	}
	else if (e.GetKey() == InputKey::w)
	{
		if (m_Tiles->CanScrollVert(+SPEED))
		{
			wdy = -SPEED;
			wdx = 0;
		}
	}
	else if (e.GetKey() == InputKey::d)
	{
		if (m_Tiles->CanScrollHoriz(+SPEED))
		{
			wdx = +SPEED;
			wdy = 0;
		}
	}
	else if (e.GetKey() == InputKey::s)
	{
		if (m_Tiles->CanScrollVert(-SPEED))
		{
			wdy = +SPEED;
			wdx = 0;
		}
	}

	m_Tiles->Scroll(wdx, wdy); 
	return true;
}


