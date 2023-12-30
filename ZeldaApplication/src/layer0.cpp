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
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
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

bool layer0::mover(KeyPressEvent& e) 
{   
	float SPEED = 4; 

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


