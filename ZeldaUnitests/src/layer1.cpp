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

	m_animator1 = MakeReference<FrameRangeAnimator>();
	m_animator1.get()->SetOnAction( 
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionLeft(); }
	);
	m_animator2 = MakeReference<FrameRangeAnimator>();
	m_animator2.get()->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionRight(); }
	); 

	Sprite link = m_Scene->CreateSprite("Link", wdx, wdy, m_WalkRight.get(), "");
	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	MovingAnimator* anim = new MovingAnimator();   
	MovingAnimation* down = new MovingAnimation("gravity", 0, 0, 0, 4);

	anim->SetOnAction([link](Animator* animator, const Animation& anim) {
		link->Move(0, +1);
	});
	link->SetMover([grid, link](Rect& r, int* dx, int* dy) {
		grid->FilterGridMotion(r, dx, dy);
		if (*dx || *dy) 
			link->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
	}); 
	link->GetGravityHandler().SetOnSolidGround([grid](Rect& r) -> bool {
		return grid->IsOnSolidGround(r);
	});
	link->GetGravityHandler().SetOnStartFalling([anim,down]() {     
		anim->Start(down, SystemClock::GetDeltaTime());
	}); 
	link->GetGravityHandler().SetOnStopFalling([anim,down]() {  
		anim->Stop();
	}); 
	link->GetGravityHandler().SetGravityAddicted(true); 


	hit	= AudioManager::Get().LoadSound("Assets/Sounds/Sound Effect (1).wav");
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
			m_Scene->GetSprite("Link")->Move(-SPEED, 0);
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
			m_Scene->GetSprite("Link")->Move(+SPEED, 0);
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
		KeyPressEvent *event = dynamic_cast<KeyPressEvent*>(&e); 
		if (event->GetKey() == InputKey::d)
			m_animator2->Start(m_walkRightAnim.get(), curr, m_walkLeftAnim.get()->GetStartFrame());
		else if (event->GetKey() == InputKey::a)
			m_animator1->Start(m_walkLeftAnim.get(), curr, m_walkLeftAnim.get()->GetStartFrame());
		else if (event->GetKey() == InputKey::q)
			AudioManager::Get().PlaySound(hit);
		else if (event->GetKey() == InputKey::m)
			AudioManager::Get().DeleteSound(hit);
	}  
	
	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		m_animator2->Stop(); 
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
	//layer.m_WalkRight->DisplayFrame(
	//	interbuff,
	//	layer.m_WalkRight->GetFrameBox(layer.m_animator2->GetCurrFrame()),
	//	{
	//		layer.m_walkRightAnim->GetDx(),
	//		layer.m_walkRightAnim->GetDy(),
	//		layer.m_WalkRight->GetFrameBox(layer.m_animator2->GetCurrFrame()).w,
	//		layer.m_WalkRight->GetFrameBox(layer.m_animator2->GetCurrFrame()).h
	//	},
	//	layer.m_animator2->GetCurrFrame()
	//);
}



