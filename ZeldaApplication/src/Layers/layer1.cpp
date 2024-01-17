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
	
	link = new Link();
	link->SetSprite(m_Scene->CreateSprite("Link", 210, 10 * 16, link->GetFilm("moving_right"), ""));

	m_movingLink = MakeReference<MovingAnimator>();
	m_movingLink->SetOnAction(
		[this](Animator* animator, const Animation& anim)
		{
			Sprite link = this->m_Scene->GetSprite("Link");
			//link->SetHasDirectMotion(true).Move(+1, 0).SetHasDirectMotion(false); 
			link->Move(+1, 0);
		}
	);

	((FrameRangeAnimator*)link->GetAnimator("moving_left"))->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionLeft(); }
	);
	((FrameRangeAnimator*)link->GetAnimator("moving_left"))->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *(link->GetAnimation("moving_left"))); }
	);

	((FrameRangeAnimator*)link->GetAnimator("moving_right"))->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionRight(); }
	);
	((FrameRangeAnimator*)link->GetAnimator("moving_right"))->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *(link->GetAnimation("moving_right")));  }
	);

	((FrameRangeAnimator*)link->GetAnimator("crouch_right"))->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionCrouchRight(); }
	);
	((FrameRangeAnimator*)link->GetAnimator("crouch_right"))->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *(link->GetAnimation("crouch_left"))); }
	);

	((FrameRangeAnimator*)link->GetAnimator("crouch_left"))->SetOnAction(
		[this](Animator* animator, const Animation& anim) { return this->FrameRangeActionCrouchLeft(); }
	);
	((FrameRangeAnimator*)link->GetAnimator("crouch_left"))->SetOnFinish(
		[this](Animator* animator) { return this->FrameRangerFinish(animator, *(link->GetAnimation("crouch_right")));  }
	);

	m_CamLeft = MakeReference<MovingAnimator>();
	m_CamRight = MakeReference <MovingAnimator>();
	m_CamLeft->SetOnAction([this](Animator* animator, const Animation& anim) { this->m_Scene->GetTiles()->Scroll(-1, 0); });
	m_CamRight->SetOnAction([this](Animator* animator, const Animation& anim) { this->m_Scene->GetTiles()->Scroll(+1, 0); });

	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	//Sprite link = m_Scene->CreateSprite("Link", 210, 10 * 16, animationFilmsMap["moving_right"].get(), "");
	link->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), link->GetSprite()));
	link->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
	link->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });

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
		KeyTapEvent* event = dynamic_cast<KeyTapEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			MovingAnimation* m = new MovingAnimation{ "moving", 0, 0, 0, 7 };
			//m_CamRight->Start(m, curr);
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("moving_right");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("moving_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("moving_right"))->GetStartFrame());
			//m_movingLink->Start(m, curr); 

			link->SetState("moving_right");
		}
		else if (event->GetKey() == InputKey::a)
		{
			MovingAnimation* m = new MovingAnimation{ "moving", 0, 0, 0, 7 };
			//m_CamLeft->Start(m, curr);
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("moving_left");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("moving_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("moving_left"))->GetStartFrame());

		//	frameRangeAnimatorsMap["moving_left"]->Start(frameRangeAnimationsMap["moving_left"].get(), curr, frameRangeAnimationsMap["moving_left"].get()->GetStartFrame());
			link->SetState("moving_left");
		}
		else if (event->GetKey() == InputKey::s)
		{
			if (link->GetState() == "moving_right") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_right"))->GetStartFrame());

				//frameRangeAnimatorsMap["crouch_right"]->Start(frameRangeAnimationsMap["crouch_right"].get(), curr, frameRangeAnimationsMap["crouch_right"].get()->GetStartFrame());
				link->SetState("crouch_right");
			}
			else if (link->GetState() == "moving_left") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_left"))->GetStartFrame());

				//frameRangeAnimatorsMap["crouch_left"]->Start(frameRangeAnimationsMap["crouch_left"].get(), curr, frameRangeAnimationsMap["crouch_left"].get()->GetStartFrame());
				link->SetState("crouch_left");
			}
		}

	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{	
			((FrameRangeAnimator*)link->GetAnimator("moving_right"))->Stop();
			//m_CamRight->Stop();
			m_movingLink->Stop();
			ENGINE_TRACE(m_Scene->GetSprite("Link")->GetFilm()->GetId());

		}
		else if (event->GetKey() == InputKey::a)
		{
			((FrameRangeAnimator*)link->GetAnimator("moving_left"))->Stop();
			//frameRangeAnimatorsMap["moving_left"]->Stop();
			//m_CamLeft->Stop();
			ENGINE_TRACE(m_Scene->GetSprite("Link")->GetFilm()->GetId());

		}
		else if (event->GetKey() == InputKey::s)
		{
			if (link->GetState() == "crouch_right") {
				Sprite link_sprite = m_Scene->GetSprite("Link");
				link_sprite->SetFilm(link->GetFilm("crouch_right"));
				link_sprite->SetFrame(0);
				link->SetState("moving_right");

			}
			else if (link->GetState() == "crouch_left") {
				Sprite link_sprite = m_Scene->GetSprite("Link");
				link_sprite->SetFilm(link->GetFilm("crouch_left"));
				link_sprite->SetFrame(0);
				link->SetState("moving_left");

			}


		}

	}

	return true;
}

void layer1::FrameRangeActionLeft()
{
//	Sprite link_sprite = m_Scene->GetSprite("Link");

	link->GetSprite()->SetFilm(link->GetFilm("moving_left"));
	link->GetSprite()->SetFrame(((FrameRangeAnimator*)link->GetAnimator("moving_left"))->GetCurrFrame());
}

void layer1::FrameRangeActionRight()
{
//	Sprite link_sprite = m_Scene->GetSprite("Link");

//	link->SetFilm(animationFilmsMap["moving_right"].get());
//	link->SetFrame(frameRangeAnimatorsMap["moving_right"]->GetCurrFrame());

	link->GetSprite()->SetFilm(link->GetFilm("moving_right"));
	link->GetSprite()->SetFrame(((FrameRangeAnimator*)link->GetAnimator("moving_right"))->GetCurrFrame());
}

void layer1::FrameRangeActionCrouchRight()
{
//	Sprite link_sprite = m_Scene->GetSprite("Link");

//	link->SetFilm(animationFilmsMap["crouch_right"].get());
//	link->SetFrame(frameRangeAnimatorsMap["crouch_right"]->GetCurrFrame());

	link->GetSprite()->SetFilm(link->GetFilm("crouch_right"));
	link->GetSprite()->SetFrame(((FrameRangeAnimator*)link->GetAnimator("crouch_right"))->GetCurrFrame());
}

void layer1::FrameRangeActionCrouchLeft()
{
//	Sprite link_sprite = m_Scene->GetSprite("Link");

	//	link->SetFilm(animationFilmsMap["crouch_left"].get());
	//	link->SetFrame(frameRangeAnimatorsMap["crouch_left"]->GetCurrFrame());

	link->GetSprite()->SetFilm(link->GetFilm("crouch_left"));
	link->GetSprite()->SetFrame(((FrameRangeAnimator*)link->GetAnimator("crouch_left"))->GetCurrFrame());
}

void layer1::FrameRangerFinish(Animator* animator, const Animation& anim)
{
	FrameRangeAnimator* a = (FrameRangeAnimator*)animator;
	FrameRangeAnimation* film = (FrameRangeAnimation*)anim.Clone();

}

