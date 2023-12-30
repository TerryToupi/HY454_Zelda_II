#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles().LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
}

void layer1::onDelete()
{
}

void layer1::onUpdate()
{
	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	Renderer::EndScene();
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
	return true;;
}

bool layer1::mover(KeyRepeatEvent& e)
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
