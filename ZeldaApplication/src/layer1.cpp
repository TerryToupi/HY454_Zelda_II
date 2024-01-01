#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
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

bool layer1::mover(KeyPressEvent& e)
{
	float SPEED = 2;

	if (e.GetKey() == InputKey::a)
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
			m_Scene->GetTiles()->Scroll(-SPEED, 0);
	}
	else if (e.GetKey() == InputKey::w)
	{
		if (m_Scene->GetTiles()->CanScrollVert(-SPEED))
			m_Scene->GetTiles()->Scroll(0, -SPEED);
	}
	else if (e.GetKey() == InputKey::d)
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(+SPEED))
			m_Scene->GetTiles()->Scroll(+SPEED, 0);
	}
	else if (e.GetKey() == InputKey::s)
	{
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	}
	return true;
}

