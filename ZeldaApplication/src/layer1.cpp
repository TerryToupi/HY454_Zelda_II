#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp"); 

	m_linkSheet = MakeReference<AnimationSheet>("link-sheet", "Assets/AnimationFilms/link-sprites.bmp"); 
	m_WalkRight = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_right.json");
	m_WalkLeft = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_Left.json"); 
	m_walkRightAnim = MakeReference<FrameRangeAnimation>("right", 0, m_WalkRight->GetTotalFrames(), 0, 2*16, 2*16, 80);
	m_walkLeftAnim = MakeReference<FrameRangeAnimation>("left", 0, m_WalkLeft->GetTotalFrames(), 0, 2*16, 2*16, 80);

	m_animator1 = MakeReference<FrameRangeAnimator>(m_Scene.get());
	m_animator1.get()->SetOnAction( 
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionLeft(); }
	);
	m_animator2 = MakeReference<FrameRangeAnimator>(m_Scene.get());
	m_animator2.get()->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionRight(); }
	); 

	Sprite link = m_Scene->CreateSprite("Link", wdx, wdy, m_WalkRight.get(), "");
}

void layer1::onDelete()
{
} 

void layer1::move(Time ts)
{
	float SPEED = 1;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
		{
			m_Scene->GetTiles()->Scroll(-SPEED, 0);  
			wdx += -SPEED; 
			m_Scene->GetSprite("Link")->SetPos(wdx, wdy);
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
			wdx += +SPEED; 
			m_Scene->GetSprite("Link")->SetPos(wdx, wdy);
		}
	}
	else if (KeyboardInput::IsPressed(SCANCODE_S))
	{
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	}
}

void layer1::onUpdate(Time ts)
{ 
	curr = ts;

	move(ts);

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	Renderer::UpdateSceneAnimators(ts); 
	Renderer::DisplaySprites();
	Renderer::EndScene();
}

void layer1::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(layer1::mover));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(layer1::mover));
}

bool layer1::mover(Event& e)
{ 
	if (KeyPressEvent::GetEventTypeStatic() == e.GetEventType())
	{ 
		KeyTapEvent *event = dynamic_cast<KeyTapEvent*>(&e); 
		if (event->GetKey() == InputKey::d)
			m_animator2->Start(m_walkRightAnim.get(), curr);
		else if (event->GetKey() == InputKey::a)
			m_animator1->Start(m_walkLeftAnim.get(), curr);
	}  
	
	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
			m_animator2->Stop();
		else if (event->GetKey() == InputKey::a)
			m_animator1->Stop();
	}

	return true;
}

void layer1::FrameRangeActionLeft()
{ 
	Sprite link = m_Scene->GetSprite("Link"); 

	link->SetFilm(m_WalkLeft.get()); 
	link->SetFrame(m_animator1->GetCurrFrame());
}

void layer1::FrameRangeActionRight()
{ 
	Sprite link = m_Scene->GetSprite("Link"); 
	
	link->SetFilm(m_WalkRight.get());
	link->SetFrame(m_animator2->GetCurrFrame());  

}

