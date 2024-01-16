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

	m_CamLeft = MakeReference<MovingAnimator>();
	m_CamRight = MakeReference <MovingAnimator>();
}

void layer0::onDelete()
{ 
	
}

void layer0::move()
{
	float SPEED = 1;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{ 
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
		{
			m_Scene->GetTiles()->Scroll(-SPEED, 0); 
		}
	}  
	else if (KeyboardInput::IsPressed(SCANCODE_W))
	{ 
		if (m_Scene->GetTiles()->CanScrollVert(-SPEED))
			m_Scene->GetTiles()->Scroll(0, -SPEED);
	}
	else if (KeyboardInput::IsPressed(SCANCODE_D))
	{ 
		if (m_Scene->GetTiles()->CanScrollHoriz(+SPEED))
		{ 
			m_Scene->GetTiles()->Scroll(+SPEED, 0);
		}
	}
	else if (KeyboardInput::IsPressed(SCANCODE_S))
	{ 
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	} 
}

void layer0::onUpdate(Time ts)
{  
	curr = ts;
	move(); 

	Renderer::BeginScene(m_Scene); 
	Renderer::DisplaySceneTiles();
	//Renderer::DebugDisplayGrid();
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
	if (KeyPressEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyTapEvent* event = dynamic_cast<KeyTapEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{ 
			//MovingAnimation* m = new MovingAnimation{ "moving", 0, 0, 0, 7 };
			//m_CamRight->Start(m, curr);
		}
		else if (event->GetKey() == InputKey::a)
		{
			//MovingAnimation* m = new MovingAnimation{ "moving", 0, 0, 0, 7 };
			//m_CamLeft->Start(m, curr);
		}
	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			//m_CamRight->Stop();
		}
		else if (event->GetKey() == InputKey::a)
		{ 
			//m_CamLeft->Stop();
		}
	}
	return true;
} 

