#include "Layer1.h"

Clipper teleportClipper;
std::pair<int, int>* bounds;
//Entity elevator;

Layer1::Layer1()
	: Layer("Layer1")
{
}

bool Layer1::cameraInStageBounds(int cameraPosition, int cameraWidth, int stageMaxLeft, int stageMaxRight) {
//	ENGINE_TRACE(cameraPosition);
	ENGINE_TRACE(stageMaxLeft);
	ENGINE_TRACE(stageMaxRight);
	return (cameraPosition > stageMaxLeft) && (cameraPosition + cameraWidth < stageMaxRight);
}

SpriteClass::Mover MakeSpriteGridLayerMover(GridLayer* gridLayer, Sprite sprite, TileLayer *tiles, std::pair<int,int> *bounds) {		//link
	return [gridLayer, sprite, tiles, bounds](Rect& r, int* dx, int* dy) {
		// the r is actually awlays the sprite->GetBox(): 
		//ENGINE_TRACE("{0}, {1}", bounds->first, bounds->second);
		int windowX = tiles->GetViewWindow().x;
		int windowWidth = tiles->GetViewWindow().w;
		int linkX = sprite->GetPosX();

		bool cameraCheck = ((linkX - windowWidth / 2 > bounds->first) && ((linkX + windowWidth / 2) < bounds->second));
	//	ENGINE_TRACE("Camera Check: {0}", cameraCheck);
		gridLayer->FilterGridMotion(r, dx, dy);
		if (*dx || *dy) 
		{
			sprite->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
			if (tiles->CanScrollHoriz(*dx) && cameraCheck)
			{	
				tiles->Scroll(linkX - windowX - windowWidth / 2, 0);
			}

		}
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
		Teleports tmp;

		tmp.origin = m_Scene->CreateSprite("tp_1" + id, p["origin"]["x"].get<int>() * 16, p["origin"]["y"].get<int>() * 16, NONPRINTABLE, "");
		tmp.origin->SetColiderBox(16, 16);
		tmp.dest = m_Scene->CreateSprite("tp_2" + id, p["destination"]["x"].get<int>() * 16, p["destination"]["y"].get<int>() * 16, NONPRINTABLE, "");
		tmp.dest->SetColiderBox(16, 16);
		tmp.stage = p["stage"].get<int>();
		m_teleports.push_back(tmp);
		id++;
	}

}

void Layer1::InitializeStages()
{
	std::ifstream stagesfile("Assets/Config/Stages/Stages.json");
	json Stages = json::parse(stagesfile);
	int max_left, max_right;

	for (auto s : Stages["stages"])
	{
		max_left = s["max-left"].get<int>()*16;
		max_right = s["max-right"].get<int>()*16;
		
		m_stages.push_back(std::make_pair(max_left, max_right));
	}

}

void Layer1::onStart()
{

	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	teleportClipper = TeleportPointClipper(m_Scene->GetTiles().get());

	link = new Link();
//	elevator = new Entity();
//	elevator->SetSprite(m_Scene->CreateSprite("Elevator", 40*16, 12*16, elevator->))
	link->SetSprite(m_Scene->CreateSprite("Link", 20 * 16, 10 * 16, link->GetFilm("moving_right"), ""));

	Rect r = m_Scene->GetTiles()->GetViewWindow();
	r.x = link->GetSprite()->GetPosX() - (r.w / 2);
	m_Scene->GetTiles()->SetViewWindow(r);
	
	InitializeTeleports();
	InitializeStages();
	m_currStage = 1;
	bounds = new std::pair<int, int>;
	*bounds = m_stages.at(0);
	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	link->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), link->GetSprite(), (m_Scene.get()->GetTiles()).get(), bounds));
	link->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
	link->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });

	MovingAnimator* anim = (MovingAnimator*)link->GetAnimator("mov_gravity");
	MovingAnimation* down = (MovingAnimation*)link->GetAnimation("mov_gravity");

	link->GetSprite()->GetGravityHandler().SetOnStartFalling([anim, down]() {
		anim->Start(down, SystemClock::GetDeltaTime());
		});
	link->GetSprite()->GetGravityHandler().SetOnStopFalling([anim, down]() {
		anim->Stop();
		});
	link->GetSprite()->GetGravityHandler().SetGravityAddicted(true);

}

void Layer1::onDelete()
{
}

void Layer1::move(Time ts)
{
	float SPEED = 1;
	
	//if (KeyboardInput::IsPressed(SCANCODE_D))
	//{
	//	int windowX = this->m_Scene->GetTiles()->GetViewWindow().x;
	//	int windowWidth = this->m_Scene->GetTiles()->GetViewWindow().w;
	//	int linkX = link->GetSprite()->GetPosX();

	//	if (this->m_Scene->GetTiles()->CanScrollHoriz((linkX - windowX - windowWidth / 2))
	//		&& this->cameraInStageBounds(linkX - windowWidth / 2, windowWidth, m_stages.at(m_currStage - 1).first, m_stages.at(m_currStage - 1).second))
	//		this->m_Scene.get()->GetTiles()->Scroll(linkX - windowX - windowWidth / 2, 0);
	//}
	//else if (KeyboardInput::IsPressed(SCANCODE_A))
	//{
	//	int windowX = this->m_Scene->GetTiles()->GetViewWindow().x;
	//	int windowWidth = this->m_Scene->GetTiles()->GetViewWindow().w;
	//	int linkX = link->GetSprite()->GetPosX();

	//	if (this->m_Scene->GetTiles()->CanScrollHoriz((linkX - windowX - windowWidth / 2))
	//		&& this->cameraInStageBounds(linkX - windowWidth / 2, windowWidth, m_stages.at(m_currStage - 1).first, m_stages.at(m_currStage - 1).second))
	//		this->m_Scene.get()->GetTiles()->Scroll(linkX - windowX - windowWidth / 2, 0);
	//}

	if (KeyboardInput::IsPressed(SCANCODE_SPACE))
	{
		MovingAnimator* tmp = (MovingAnimator*)link->GetAnimator("mov_jumping");
		tmp->Start((MovingAnimation*)link->GetAnimation("mov_jumping"), SystemClock::GetDeltaTime());
	}
}

void Layer1::onUpdate(Time ts)
{
	curr = ts;
	move(ts);
	*bounds = m_stages.at(m_currStage - 1);
	
	TeleportCheck();

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

			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_moving_right");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"))->GetStartFrame());
			
			link->SetState("moving");
			link->SetLookingAt("right");
			((MovingAnimator*)link->GetAnimator("mov_moving_right"))->Start((MovingAnimation*)link->GetAnimation("mov_moving_right"), SystemClock::GetDeltaTime());

		//	m_CameraRight->Start(m_CameraRightAnim, SystemClock::GetDeltaTime());

		}
		if (event->GetKey() == InputKey::a)
		{	

			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_moving_left");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"))->GetStartFrame());
			
			link->SetState("moving");
			link->SetLookingAt("left");
			((MovingAnimator*)link->GetAnimator("mov_moving_left"))->Start((MovingAnimation*)link->GetAnimation("mov_moving_left"), SystemClock::GetDeltaTime());

		//	m_CameraLeft->Start(m_CameraLeftAnim, SystemClock::GetDeltaTime());

		}
		if (event->GetKey() == InputKey::s)
		{
			if (link->GetLookingAt() == "right") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_crouch_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"))->GetStartFrame());
				
				link->SetState("crouch");
			}
			else if (link->GetLookingAt() == "left") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_crouch_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"))->GetStartFrame());
				
				link->SetState("crouch");
			}
		}
		if (event->GetKey() == InputKey::q)
		{
			if (link->GetLookingAt() == "right" && (link->GetState() == "crouch" || link->GetState() == "crouch_attacking")) {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_crouch_attack_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"))->GetStartFrame());

				link->SetState("crouch_attacking");
			}
			else if (link->GetLookingAt() == "left" && (link->GetState() == "crouch" || link->GetState() == "crouch_attacking")) {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_crouch_attack_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"))->GetStartFrame());

				link->SetState("crouch_attacking");
			}
			else if (link->GetLookingAt() == "right"){
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_attacking_right");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"))->GetStartFrame());

				link->SetState("attacking");
			}
			else if (link->GetLookingAt() == "left") {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_attacking_left");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"))->GetStartFrame());

				link->SetState("attacking");
			}
		}
	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{	
			((FrameRangeAnimator*)link->GetAnimator("frame_moving_right"))->Stop();
			((MovingAnimator*)link->GetAnimator("mov_moving_right"))->Stop();
		//	m_CameraRight->Stop();
		}

		if (event->GetKey() == InputKey::a)
		{
			((FrameRangeAnimator*)link->GetAnimator("frame_moving_left"))->Stop();
			((MovingAnimator*)link->GetAnimator("mov_moving_left"))->Stop();
		//	m_CameraLeft->Stop();
		}

		if (event->GetKey() == InputKey::s)
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

void Layer1::TeleportCheck()
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_teleports.at(0).origin->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	for (auto i : m_teleports)
	{	
		tmpBox = i.origin->GetBox();
		if (teleportClipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{	
			Sprite dest = i.dest;
			m_Scene->GetColider().Register(link_sprite, i.origin, [link_sprite, dest, tilelayer, this, i](Sprite s1, Sprite s2) {
				link_sprite->SetPos(dest->GetPosX(), dest->GetPosY()); 
				m_currStage = i.stage;
				ENGINE_TRACE(i.stage);
				int32_t dx = dest->GetPosX() - tilelayer->GetViewWindow().x - (tilelayer->GetViewWindow().w / 2);
				ENGINE_TRACE("Teleport Pos: {0}, {1}", dest->GetPosX()/16, dest->GetPosY()/16);
				if (tilelayer->CanScrollHoriz(dx))
					tilelayer->Scroll(dx,0);
				});

			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.origin);

		}
	}

}


