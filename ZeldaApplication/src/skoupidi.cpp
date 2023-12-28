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
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(skoupidi::mover));
	dispatcher.Dispatch<KeyRepeatEvent>(APP_EVENT_FUNTION(skoupidi::mover));
} 

bool skoupidi::mover(KeyTapEvent& e) 
{   
	float SPEED = 5; 

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

bool skoupidi::mover(KeyRepeatEvent& e)
{
	float SPEED = 5;

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


