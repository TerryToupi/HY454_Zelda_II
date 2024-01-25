#include "Layer1.h"
#include "Layer1.h"

Clipper clipper;
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

const Clipper InitClipper(TileLayer* layer)
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

void Layer1::InitializeEnemies(GridLayer *grid) 
{
	std::ifstream wosuFile("Assets/Config/Enemies/wosu_config.json");
	json Wosus = json::parse(wosuFile);
	ID i = 0;

	for (auto w : Wosus["data"])
	{
		ID id = UUID::GenerateUUID();

		m_enemies.emplace(std::make_pair( i, new Wosu(i, w["lookingAt"].get<std::string>(), w["stage"].get<uint32_t>())));
		ENGINE_TRACE(w["max_x"].get<uint32_t>());
		m_enemies.at(i)->SetMaxX(w["max_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetMinX(w["min_x"].get<uint32_t>() * 16);
	    m_enemies.at(i)->SetSprite((m_Scene->CreateSprite("Wosu" + std::to_string(id), w["spawn_pos"]["x"].get<uint32_t>() * 16, w["spawn_pos"]["y"].get<uint32_t>() * 16, m_enemies.at(i)->GetFilm("moving_" + w["lookingAt"].get<std::string>()), "")));
		m_enemies.at(i)->GetSprite()->SetColiderBox(16, 32);
		m_enemies.at(i)->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), m_enemies.at(i)->GetSprite()));
		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });
		
		MovingAnimator* anim = (MovingAnimator*)m_enemies.at(i)->GetAnimator("mov_gravity");
		MovingAnimation* down = (MovingAnimation*)m_enemies.at(i)->GetAnimation("mov_gravity");

		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnStartFalling([anim, down]() {
			anim->Start(down, SystemClock::GetDeltaTime());
			});

		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnStopFalling([anim, down]() {
			anim->Stop();
			});

		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetGravityAddicted(true);

		i++;
	}
}

void Layer1::InitializeAudio()
{	
	AudioID tmp = AudioManager::Get().LoadSound("Assets/Sounds/Link/attacking_sound.wav");
	m_sounds.emplace(std::make_pair("attacking", tmp));
}

  
void Layer1::UpdateSpell(Spell& spell, Time ts) {
	if (!spell.canUse()) {
		if (spell.getDurationRemainingTime() == 0) {
			spell.setCooldownRemainingTime(spell.getCooldownRemainingTime() - ts);

			if (spell.getCooldownRemainingTime() <= 0) {
				spell.setCooldownRemainingTime(0);
			}

			//ENGINE_TRACE(spell.getCooldownRemainingTime());
		}
		else {
			spell.setDurationRemainingTime(spell.getDurationRemainingTime() - ts);

			if (spell.getDurationRemainingTime() <= 0) {
				spell.setDurationRemainingTime(0);

				if (spell.getType() == "jumpspell") {
					link->setJumpingForce(link->getJumpingForce() / 2);
					link->EraseAnimation(link->jumpAnimation);
					link->jumpAnimation = new MovingAnimation("mov_jumping", link->getJumpingForce(), 0, 0, 20);
					link->EmplaceAnimation(link->jumpAnimation);
				}
			}

			//ENGINE_TRACE(spell.getDurationRemainingTime());
		}
	}
}

void Layer1::CheckTimers(Time ts) {
	static Time prevTs = 0;

	UpdateSpell(link->lifespell, ts);
	UpdateSpell(link->jumpspell, ts);
	UpdateSpell(link->shieldspell, ts);

	if (link->getDamageCoolDown() != 0)
		link->setDamageCoolDown(link->getDamageCoolDown() - ts);
	if (link->getDamageCoolDown() < 0)
		link->setDamageCoolDown(0);
	
	prevTs = ts;
}

void Layer1::onStart()
{	
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	clipper = InitClipper(m_Scene->GetTiles().get());

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
	

	InitializeEnemies(grid); 
}

void Layer1::onDelete()
{

}

void Layer1::onUpdate(Time ts)
{
	*bounds = m_stages.at(m_currStage - 1);
	
	TeleportHandler();
	
	EnemyMovement();
	EnemyHandler();

	CheckTimers(ts);
	//ENGINE_TRACE(link->getDamageCoolDown());

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	Renderer::DisplaySprites();
	//Renderer::DebugDisplayGrid();
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

		if (event->GetKey() == InputKey::NUM_1)
		{

			if (link->getMagicPoints() >= link->lifespell.getCost() && link->lifespell.canUse()) {
				link->setMagicPoints(link->getMagicPoints() - link->lifespell.getCost());
				link->setHealth(link->getHealth() + 50);
				link->lifespell.setDurationRemainingTime(link->lifespell.getDuration());
				link->lifespell.setCooldownRemainingTime(link->lifespell.getCooldown());
			}
		}

		if (event->GetKey() == InputKey::NUM_2)
		{

			if (link->getMagicPoints() >= link->jumpspell.getCost() && link->jumpspell.canUse()) {
				link->setMagicPoints(link->getMagicPoints() - link->jumpspell.getCost());
				link->setJumpingForce(link->getJumpingForce() * 2);
				link->jumpspell.setDurationRemainingTime(link->jumpspell.getDuration());
				link->jumpspell.setCooldownRemainingTime(link->jumpspell.getCooldown());

				link->EraseAnimation(link->jumpAnimation);
				
				link->jumpAnimation = new MovingAnimation("mov_jumping", link->getJumpingForce(), 0, 0, 12);
				link->EmplaceAnimation(link->jumpAnimation);
				
			}

		}

		if (event->GetKey() == InputKey::NUM_3)
		{

			if (link->getMagicPoints() >= link->shieldspell.getCost() && link->shieldspell.canUse()) {
				link->setMagicPoints(link->getMagicPoints() - link->shieldspell.getCost());
				link->shieldspell.setDurationRemainingTime(link->shieldspell.getDuration());
				link->shieldspell.setCooldownRemainingTime(link->shieldspell.getCooldown());
			}

		}
	}

	if (KeyReleaseEvent::GetEventTypeStatic() == e.GetEventType())
	{
		KeyReleaseEvent* event = dynamic_cast<KeyReleaseEvent*>(&e);
		if (event->GetKey() == InputKey::d)
		{
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
			((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();
			link->SetState("idle");
		}
		
		if (event->GetKey() == InputKey::a)
		{
			((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
			((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();
			link->SetState("idle");
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

void Layer1::TeleportHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_teleports.at(0).origin->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	for (auto i : m_teleports)
	{	
		tmpBox = i.origin->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
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

void Layer1::EnemyMovement() {
	for (auto e : m_enemies)
	{	
		Rect d1;
		Rect d2;
		Rect tmpBox = e.second->GetSprite()->GetBox();
		MovingAnimator* mov = (MovingAnimator*)e.second->GetAnimator("mov_moving");
		FrameRangeAnimator* anim = (FrameRangeAnimator*)e.second->GetAnimator("frame_animator");
		
		if (e.second->GetStage() == m_currStage)
		{
			if (e.second->GetState() != "death")
			{
				e.second->SetState("moving");

				if (e.second->GetSprite()->GetPosX() > e.second->GetMaxX())
				{
					e.second->SetLookingAt("left");
					anim->Stop();
				}
				else if (e.second->GetSprite()->GetPosX() < e.second->GetMinX())
				{
					e.second->SetLookingAt("right");
					anim->Stop();
				}

				if (anim->HasFinished())
					anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()))->GetStartFrame());
				if (mov->HasFinished())
					mov->Start((MovingAnimation*)e.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
			}
		}
		else {
			e.second->SetState("idle");

			if (!mov->HasFinished())
				mov->Stop();
			
			if (!anim->HasFinished())
				anim->Stop();
		}
	}
}

void Layer1::EnemyHandler() 
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_enemies.begin()->second->GetSprite()->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();
	Enemy* dying = nullptr;

	for (auto i : m_enemies)
	{
		tmpBox = i.second->GetSprite()->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2) && link->getDamageCoolDown() == 0)
		{
			m_Scene->GetColider().Register(link_sprite, i.second->GetSprite(), [link_sprite, tilelayer, this, i](Sprite s1, Sprite s2) {
				int32_t dx = (link->GetLookingAt() == "right") ? -16 : 16;
				FrameRangeAnimator* anim = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				MovingAnimator* mov = (MovingAnimator*)link->GetAnimator("mov_damage");

				if (link->GetState() != "attacking" && link->GetState() != "crouch_attack")
				{

					link->setDamageCoolDown(2000);
					if (link->GetState() == "crouch")
					{
						if (link->GetLookingAt() == i.second->GetLookingAt())
						{
							link->SetState("damage_from");

							if (!anim->HasFinished())
								anim->Stop();

							anim->Start((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()))->GetStartFrame());
						}

					}
					else
					{
						link->SetState("damage_from");

						if (!anim->HasFinished())
							anim->Stop();

						anim->Start((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()))->GetStartFrame());
					}

					if (mov->HasFinished())
						mov->Start((MovingAnimation*)link->GetAnimation("mov_damage"), SystemClock::GetDeltaTime());

					anim->Start((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()))->GetStartFrame());
					link->GetSprite()->Move(dx, -5);
					link->takeDamage(i.second->GetDamage());

				}
				else
				{
					i.second->TakeDamage(8);
				}

				});

			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.second->GetSprite());

		}

		if (i.second->GetHealth() == 0)
		{
			FrameRangeAnimator* anim = (FrameRangeAnimator*)i.second->GetAnimator("frame_animator");
			MovingAnimator* mov = (MovingAnimator*)i.second->GetAnimator("mov_moving");

			if (!mov->HasFinished())
				mov->Stop();

			if (!anim->HasFinished())
				anim->Stop();

			i.second->SetLookingAt("");
			i.second->SetState("death");
			

			anim->Start((FrameRangeAnimation*)i.second->GetAnimation("frame_death"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)i.second->GetAnimation("frame_death"))->GetStartFrame());
			dying = i.second;
		}
	}

	if (dying)
		m_enemies.erase(m_enemies.find(dying->GetID()));

}
