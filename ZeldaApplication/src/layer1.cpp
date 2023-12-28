#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>();
	m_Tiles = MakeReference<TileLayer>(1);
	m_Tiles->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
}

void layer1::onDelete()
{
}

void layer1::onUpdate()
{
	auto& fb = Renderer::FrameBufferInstance().GetBackBuffer();
	Rect disp{ 0, 0, fb.GetWidth(), fb.GetHeight() };
	m_Tiles->Display(fb, disp);
}

void layer1::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(layer1::mover));
	dispatcher.Dispatch<KeyRepeatEvent>(APP_EVENT_FUNTION(layer1::mover));
}

bool layer1::mover(KeyTapEvent& e)
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

bool layer1::mover(KeyRepeatEvent& e)
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
