#include "skoupidi.h"

#define THE_GREATES_MPAMPIS "Mpampis"

using namespace Engine;

skoupidi::skoupidi()
	: Layer("skoupidi")
{ 
}

void skoupidi::onAttach()
{
	m_Scene = MakeReference<Scene>(); 
	Sprite nai = m_Scene->CreateSprite(THE_GREATES_MPAMPIS);
}

void skoupidi::onDettach()
{ 
	
}

void skoupidi::onUpdate()
{ 
}

void skoupidi::onEvent(Event& e)
{   
	if (e.GetEventType() == KeyTapEvent::GetEventTypeStatic())
	{  
		auto key = static_cast<KeyTapEvent&>(e);  
		if (key.GetKey() == InputKey::a)
		{
			Sprite nai = m_Scene->GetSprite(THE_GREATES_MPAMPIS); 
		}
	}
}  
