#include "Layer1.h"

Clipper teleportClipper;
std::pair<int, int>* bounds;

Layer1::Layer1()
	: Layer("Layer1")
{
}

SpriteClass::Mover MakeSpriteGridLayerMoverLink(GridLayer* gridLayer, Sprite sprite, TileLayer *tiles, std::pair<int,int> *bounds) {
	return [gridLayer, sprite, tiles, bounds](Rect& r, int* dx, int* dy) {
		int windowX = tiles->GetViewWindow().x;
		int windowWidth = tiles->GetViewWindow().w;
		int linkX = sprite->GetPosX();

		bool cameraCheck = ((linkX - windowWidth / 2 > bounds->first) && ((linkX + windowWidth / 2) < bounds->second));
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

SpriteClass::Mover MakeSpriteGridLayerMover(GridLayer* gridLayer, Sprite sprite) {
	return [gridLayer, sprite](Rect& r, int* dx, int* dy) {
		gridLayer->FilterGridMotion(r, dx, dy);
		if (*dx || *dy)
		{
			sprite->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
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

void Layer1::InitializeAudio()
{	
	AudioID tmp = AudioManager::Get().LoadSound("Assets/Sounds/Link/attacking_sound.wav");
	m_sounds.emplace(std::make_pair("attacking", tmp));
}

void Layer1::onStart()
{	
	
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	teleportClipper = TeleportPointClipper(m_Scene->GetTiles().get());

	link = new Link();
	link->SetSprite(m_Scene->CreateSprite("Link", 20 * 16, 10 * 16, link->GetFilm("moving_right"), ""));
	link->GetSprite()->SetColiderBox(16, 32);

	Rect r = m_Scene->GetTiles()->GetViewWindow();
	r.x = link->GetSprite()->GetPosX() - (r.w / 2);
	m_Scene->GetTiles()->SetViewWindow(r);
	
	InitializeTeleports();
	InitializeStages();
	InitializeAudio();
	m_currStage = 1;
	bounds = new std::pair<int, int>;
	*bounds = m_stages.at(0);
	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	link->GetSprite()->SetMover(MakeSpriteGridLayerMoverLink(m_Scene->GetTiles()->GetGrid(), link->GetSprite(), (m_Scene.get()->GetTiles()).get(), bounds));
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
	
//	ID id = UUID::GenerateUUID();
//	m_enemies.emplace(std::make_pair(id, Wosu(id, "left")));
	wosu = new Wosu(1, "left");
	anim = (MovingAnimator*)wosu->GetAnimator("mov_gravity");
	down = (MovingAnimation*)wosu->GetAnimation("mov_gravity");

	wosu->SetSprite(m_Scene->CreateSprite("Wosu", 30 * 16, 10 * 16, wosu->GetFilm("moving_left"), ""));

	wosu->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), wosu->GetSprite()));
	wosu->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
	wosu->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });

	wosu->GetSprite()->GetGravityHandler().SetOnStartFalling([anim, down]() {
		anim->Start(down, SystemClock::GetDeltaTime());
		});

	wosu->GetSprite()->GetGravityHandler().SetOnStopFalling([anim, down]() {
		anim->Stop();
		});

	wosu->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
}

void Layer1::onDelete()
{
}

void Layer1::onUpdate(Time ts)
{
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
			link->SetState("moving");
			link->SetLookingAt("right");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"))->GetStartFrame());
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Start((MovingAnimation*)link->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
			
			wosu->SetState("moving");
			tmp = (FrameRangeAnimator*)wosu->GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)wosu->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)wosu->GetAnimation("frame_moving_right"))->GetStartFrame());
			((MovingAnimator*)wosu->GetAnimator("mov_moving"))->Start((MovingAnimation*)wosu->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
		}
		
		if (event->GetKey() == InputKey::a)
		{
			link->SetState("moving");
			link->SetLookingAt("left");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"))->GetStartFrame());
			
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Start((MovingAnimation*)link->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
		}
		
		if (event->GetKey() == InputKey::s)
		{
			if (link->GetLookingAt() == "right") {
				link->SetState("crouch");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"))->GetStartFrame());
			}
			else if (link->GetLookingAt() == "left") {
				link->SetState("crouch");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"))->GetStartFrame());
			}
		}

		if (event->GetKey() == InputKey::q)
		{
			if (link->GetLookingAt() == "right" && (link->GetState() == "crouch" || link->GetState() == "crouch_attack")) {
				link->SetState("crouch_attack");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"))->GetStartFrame());
			}
			else if (link->GetLookingAt() == "left" && (link->GetState() == "crouch" || link->GetState() == "crouch_attack")) {

				link->SetState("crouch_attack");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"))->GetStartFrame());

			}
			else if (link->GetLookingAt() == "right") {
				link->SetState("attacking");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"))->GetStartFrame());
			}
			else if (link->GetLookingAt() == "left") {
				link->SetState("attacking");
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"))->GetStartFrame());
			}
			AudioManager::Get().PlaySound(m_sounds.at("attacking"));
		}

		if (event->GetKey() == InputKey::SPACE)
		{
			MovingAnimator* tmp = (MovingAnimator*)link->GetAnimator("mov_jumping");
			tmp->Start((MovingAnimation*)link->GetAnimation("mov_jumping"), SystemClock::GetDeltaTime());
			link->SetState("jumping");
		}
	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
			((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();

			((MovingAnimator*)wosu->GetAnimator("mov_moving"))->Stop();
			((FrameRangeAnimator*)wosu->GetAnimator("frame_animator"))->Stop();
		}
		
		if (event->GetKey() == InputKey::a)
		{
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
			((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();
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


