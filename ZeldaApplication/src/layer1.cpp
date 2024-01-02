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

void layer1::move()
{ 
	float SPEED = 1;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
			m_Scene->GetTiles()->Scroll(-SPEED, 0);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_W)) 
	{
		if (m_Scene->GetTiles()->CanScrollVert(-SPEED))
			m_Scene->GetTiles()->Scroll(0, -SPEED);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_D))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(+SPEED))
			m_Scene->GetTiles()->Scroll(+SPEED, 0);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_S))
	{
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	}
}

void layer1::onUpdate()
{ 
	move();

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
	return true;
}

