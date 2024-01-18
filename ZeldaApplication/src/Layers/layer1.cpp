#include "Layer1.h"

Clipper teleportClipper;

Layer1::Layer1()
	: Layer("Layer1")
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

const Clipper TeleportPointClipper(TileLayer* layer)
{
	return Clipper().SetView(
		[layer](void) -> const Rect&
		{ return layer->GetViewWindow(); }
	);
}

void Layer1::InitializeTeleports()
{
	std::ifstream file("Assets/Config/TeleportPoints/TeleportPoints.json");
	json Points = json::parse(file);
	Sprite origin, dest;
	char id = 'A';

	for (auto p : Points["points"])
	{	
		origin = m_Scene->CreateSprite("tp_1" + id, p["origin"]["x"] * 16, p["origin"]["y"] * 16, NONPRINTABLE, "");
		origin->SetColiderBox(32, 32);
		dest = m_Scene->CreateSprite("tp_2" + id, p["destination"]["x"] * 16, p["destination"]["y"] * 16, NONPRINTABLE, "");
		dest->SetColiderBox(32, 32);
		m_teleports.push_back(std::make_pair(origin, dest));
		id++;
	}

}

void Layer1::onStart()
{

	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	teleportClipper = TeleportPointClipper(m_Scene->GetTiles().get());

	link = new Link();
	link->SetSprite(m_Scene->CreateSprite("Link", 210, 10 * 16, link->GetFilm("moving_right"), ""));
	ENGINE_TRACE(link->GetLookingAt() + " " + link->GetState());
	
	InitializeTeleports();
	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	link->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), link->GetSprite()));
	link->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
	link->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });

	Sprite waypoint = m_Scene->CreateSprite("w1", 132, 8 * 16, NONPRINTABLE, "");
	waypoint->SetColiderBox(16, 32);
}

void Layer1::onDelete()
{
}

void Layer1::move(Time ts)
{
	float SPEED = 1;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
		{
			m_Scene->GetTiles()->Scroll(-SPEED, 0);
		}
		m_Scene->GetSprite("Link")->Move(-SPEED, 0);
		CanTeleport();

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
		}
		m_Scene->GetSprite("Link")->Move(+SPEED, 0);
		CanTeleport();
	}
	else if ((KeyboardInput::IsPressed(SCANCODE_S)))
	{
		m_Scene->GetSprite("Link")->Move(0, +SPEED);
	}
	//else if (KeyboardInput::IsPressed(SCANCODE_SPACE))
	//{
	//	link->GetSprite()->SetMotionQuantizerUse(true);
	//	link->GetSprite()->SetQuanntizerHeightVelocity(5,2*16);
	//	link->GetSprite()->Move(0, -5);
	//	link->GetSprite()->SetMotionQuantizerUse(false);
	//}
}

void Layer1::onUpdate(Time ts)
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

void Layer1::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(Layer1::mover));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(Layer1::mover));
}

bool Layer1::mover(Event& e)
{
	if (KeyPressEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyTapEvent* event = dynamic_cast<KeyTapEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("moving_right");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("moving_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("moving_right"))->GetStartFrame());

			link->SetState("moving");
			link->SetLookingAt("right");

			ENGINE_TRACE(link->GetLookingAt() + " " + link->GetState());

		}
		else if (event->GetKey() == InputKey::a)
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("moving_left");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("moving_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("moving_left"))->GetStartFrame());

			link->SetState("moving");
			link->SetLookingAt("left");

			ENGINE_TRACE(link->GetLookingAt() + " " + link->GetState());

		}
		else if (event->GetKey() == InputKey::s)
		{
			if (link->GetLookingAt() == "right") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_right"))->GetStartFrame());
				
				link->SetState("crouch");
			}
			else if (link->GetLookingAt() == "left") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_left"))->GetStartFrame());
				
				link->SetState("crouch");
			}
		}
		else if (event->GetKey() == InputKey::q)
		{
			if (link->GetLookingAt() == "right" && (link->GetState() == "crouch" || link->GetState() == "crouch_attacking")) {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_attack_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_attack_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_attack_right"))->GetStartFrame());

				link->SetState("crouch_attacking");
			}
			else if (link->GetLookingAt() == "left" && (link->GetState() == "crouch" || link->GetState() == "crouch_attacking")) {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("crouch_attack_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("crouch_attack_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("crouch_attack_left"))->GetStartFrame());

				link->SetState("crouch_attacking");
			}
			else if (link->GetLookingAt() == "right"){
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("attacking_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("attacking_right"), curr, ((FrameRangeAnimation*)link->GetAnimation("attacking_right"))->GetStartFrame());

				link->SetState("attacking");
			}
			else if (link->GetLookingAt() == "left") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("attacking_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("attacking_left"), curr, ((FrameRangeAnimation*)link->GetAnimation("attacking_left"))->GetStartFrame());

				link->SetState("attacking");
			}
		}
		else if (event->GetKey() == InputKey::SPACE)
		{
			MovingAnimator* tmp = (MovingAnimator*)link->GetAnimator("jumping");
			tmp->Start((MovingAnimation*)link->GetAnimation("jumping"), curr);
			link->SetState("jumping");
		}
	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{	
			((FrameRangeAnimator*)link->GetAnimator("moving_right"))->Stop();

		}
		else if (event->GetKey() == InputKey::a)
		{
			((FrameRangeAnimator*)link->GetAnimator("moving_left"))->Stop();

		}
		else if (event->GetKey() == InputKey::s)
		{
			if (link->GetLookingAt() == "right") {
				Sprite link_sprite = m_Scene->GetSprite("Link");
				link_sprite->SetFilm(link->GetFilm("crouch_right"));
				link_sprite->SetFrame(0);
				link->SetState("moving");
			}
			else if (link->GetLookingAt() == "left") {
				Sprite link_sprite = m_Scene->GetSprite("Link");
				link_sprite->SetFilm(link->GetFilm("crouch_left"));
				link_sprite->SetFrame(0);
				link->SetState("moving");
			}
		}
	}

	return true;
}




void Layer1::CanTeleport()
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_teleports.at(0).first->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	for (auto i : m_teleports)
	{	
		tmpBox = i.first->GetBox();
		if (teleportClipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{	
			Sprite dest = i.second;
			m_Scene->GetColider().Register(link_sprite, i.first, [link_sprite, dest, tilelayer](Sprite s1, Sprite s2) {
				link_sprite->SetPos(dest->GetPosX(), dest->GetPosY()); 
				
				int32_t dx = dest->GetPosX() - tilelayer->GetViewWindow().x - (tilelayer->GetViewWindow().w / 2);

				//if (tilelayer->CanScrollHoriz(dx))
				//	tilelayer->Scroll(dx,0);
				});

			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.first);

		}
	}

}
