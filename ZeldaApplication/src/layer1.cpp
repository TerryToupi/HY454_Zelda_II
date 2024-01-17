#include "layer1.h"

layer1::layer1()
	: Layer("layer1")
{
}

SpriteClass::Mover MakeSpriteGridLayerMover(GridLayer* gridLayer, Sprite sprite) {
	return [gridLayer, sprite](Rect& r, int* dx, int* dy) {
		// the r is actually awlays the sprite->GetBox(): 
		gridLayer->FilterGridMotion(r, dx, dy);
		if (*dx || *dy)
			sprite->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
		};
}

void layer1::onStart()
{
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");

	m_linkSheet = MakeReference<AnimationSheet>("link-sheet", "Assets/AnimationFilms/link-sprites.bmp");
	m_WalkRight = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_right.json");
	m_WalkLeft = MakeReference<AnimationFilm>(m_linkSheet.get(), "Assets/Config/Animations/Link/moving_Left.json");
	m_walkRightAnim = MakeReference<FrameRangeAnimation>("right", 0, m_WalkRight->GetTotalFrames(), 0, 300, 12 * 16, 50);
	m_walkLeftAnim = MakeReference<FrameRangeAnimation>("left", 0, m_WalkLeft->GetTotalFrames(), 0, 300, 12 * 16, 50);  

	m_movingLink = MakeReference<MovingAnimator>();
	m_movingLink->SetOnAction(
		[this](Animator* animator, const Animation& anim)
		{
			Sprite link = this->m_Scene->GetSprite("Link");
			//link->SetHasDirectMotion(true).Move(+1, 0).SetHasDirectMotion(false); 
			link->Move(+1, 0);
		}
	);

	m_animator1 = MakeReference<FrameRangeAnimator>();
	m_animator1.get()->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionLeft(); }
	);
	m_animator1.get()->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *m_walkLeftAnim.get()); }
	);
	m_animator2 = MakeReference<FrameRangeAnimator>();
	m_animator2.get()->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionRight(); }
	);
	m_animator2.get()->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *m_walkRightAnim.get()); }
	);

	m_CamLeft = MakeReference<MovingAnimator>();
	m_CamRight = MakeReference <MovingAnimator>();
	m_CamLeft->SetOnAction([this](Animator* animator, const Animation& anim) { this->m_Scene->GetTiles()->Scroll(-1, 0);});
	m_CamRight->SetOnAction([this](Animator* animator, const Animation& anim) { this->m_Scene->GetTiles()->Scroll(+1, 0);});

	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	Sprite link = m_Scene->CreateSprite("Link", 210, 10*16, m_WalkRight.get(), ""); 
	link->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), link)); 
	link->GetGravityHandler().SetGravityAddicted(true);
	link->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });  

	Sprite waypoint = m_Scene->CreateSprite("w1", 132, 8 * 16, NONPRINTABLE, ""); 
	waypoint->SetColiderBox(5, 32);
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
		{
			m_Scene->GetTiles()->Scroll(0, -SPEED);
		} 
		//m_Scene->GetSprite("Link")->SetMotionQuantizerUse(true); 
		//m_Scene->GetSprite("Link")->SetQuanntizerHeightVelocity(5, 5);
		m_Scene->GetSprite("Link")->Move(0, -SPEED); 
		//m_Scene->GetSprite("Link")->SetMotionQuantizerUse(false); 
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
		{
			m_Scene->GetTiles()->Scroll(0, +SPEED);
		}
			m_Scene->GetSprite("Link")->Move(0, +SPEED);
	}
}

void layer1::onUpdate(Time ts)
{
	curr = ts;
	move(ts);

	Sprite link = m_Scene->GetSprite("Link");
	Sprite w1 = m_Scene->GetSprite("w1");
	m_Scene->GetColider().Register(link, w1, [link, w1](Sprite s1, Sprite s2) { ENGINE_TRACE("EKOUTOULISANE\n"); w1->Destroy(); });
	m_Scene->GetColider().Check();
	m_Scene->GetColider().Cancel(link, w1);

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	//Renderer::DebugDisplayGrid();
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
		{ 
			MovingAnimation *m = new MovingAnimation {"moving", 0, 0, 0, 7};
			//m_CamRight->Start(m, curr);
			m_animator2->Start(m_walkRightAnim.get(), curr, m_walkRightAnim.get()->GetStartFrame()); 
			m_movingLink->Start(m, curr); 
		}
		else if (event->GetKey() == InputKey::a)
		{ 
			MovingAnimation *m = new MovingAnimation {"moving", 0, 0, 0, 7}; 
			//m_CamLeft->Start(m, curr);
			m_animator1->Start(m_walkLeftAnim.get(), curr, m_walkRightAnim.get()->GetStartFrame());
		}
	}  
	
	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			//m_CamRight->Stop();
			m_animator2->Stop(); 
			m_movingLink->Stop();
		}
		else if (event->GetKey() == InputKey::a)
		{ 
			//m_CamLeft->Stop();
			m_animator1->Stop();
		}
	}

	return false;
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

void layer1::FrameRangerFinish(Animator* animator, const Animation& anim)
{ 
	FrameRangeAnimator* a = (FrameRangeAnimator*)animator;   
	FrameRangeAnimation* film = (FrameRangeAnimation*)anim.Clone(); 

	//int currF = a->GetCurrFrame();   
	//film->SetReps(1);

	//if (currF != film->GetStartFrame())
	//{  
	//	ENGINE_CORE_TRACE("currf: {0}, end film: {1}", currF, film->GetEndFrame());
	//	a->Start(film, curr, currF);
	//}  

	//delete film; 
}

