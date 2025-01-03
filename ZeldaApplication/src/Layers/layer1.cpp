#include "Layer1.h"

Clipper clipper;
std::pair<int, int>* bounds;
Elevator* currElevator;
GridLayer* gridlayer;

DeviceID backgroundMusic;
bool pausedMusic;

SpriteClass::Mover MakeSpriteGridLayerMoverLink(GridLayer* gridLayer, Sprite sprite, TileLayer* tiles, std::pair<int, int>* bounds) {
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
		if (sprite->GetTypeId() != "W_ARROW")
			gridLayer->FilterGridMotion(r, dx, dy);
		if (*dx || *dy)
		{
			sprite->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
		}
		};
}

SpriteClass::Mover MakeSpriteGridLayerMoverElevator(GridLayer* gridLayer, Sprite sprite)
{
	return [gridLayer, sprite](Rect& r, int* dx, int* dy) {
		gridLayer->FilterGridMotion(r, 0, dy);
		if (*dy)
		{
			sprite->SetHasDirectMotion(true).Move(0, *dy).SetHasDirectMotion(false);
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

void InitilizeLayer(Layer1* layer)
{
	layer->LoadSheets();
	layer->InitialiazeCollectibles();
	layer->InitializeAudio();
	layer->InitializeDoors();
	layer->InitializeStages();
	layer->InitializeTeleports();
	layer->InitializeBridge();
}

void PlayElevatorSound(Layer1* layer)
{
	uint32_t delay = 0;
	while (currElevator->GetState() == "moving") 
	{
		if (delay == 0)
		{
			AudioManager::Get().PlaySound(layer->m_sounds.at("elevator"));
			delay = 10000;
		}
		else
			delay -= SystemClock::GetDeltaTimeStep();
	}
	return;
}

void ArrowFrameRangeStart(Layer1* layer, Arrow* arrow)
{
	std::string film = arrow->GetState() + "_" + arrow->GetLookingAt();
	arrow->GetSprite()->SetFilm(arrow->GetFilm(film));

	layer->m_Scene->GetColider().Register(layer->link->GetSprite(), arrow->GetSprite(), [layer, arrow](Sprite s1, Sprite s2) {
		
		if (!arrow->IsCollided())
		{
			FrameRangeAnimator* anim = (FrameRangeAnimator*)layer->link->GetAnimator("frame_animator");

			MovingAnimator* mov = (MovingAnimator*)layer->link->GetAnimator("mov_damage");
			AudioManager::Get().PlaySound(layer->m_sounds.at("link_damage"));

			if (!anim->HasFinished())
				anim->Stop();

			if (!mov->HasFinished())
				mov->Stop();

			layer->link->SetState("damage_from");

			mov->Start((MovingAnimation*)layer->link->GetAnimation("mov_damage"), SystemClock::GetDeltaTime());
			anim->Start((FrameRangeAnimation*)layer->link->GetAnimation("frame_damage_from_" + layer->link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)layer->link->GetAnimation("frame_damage_from_" + layer->link->GetLookingAt()))->GetStartFrame());
			layer->link->takeDamage(5);

			arrow->SetCollided(true);
		}
	});
}

void ArrowFrameRangeAction(Layer1* layer, Arrow* arrow, FrameRangeAnimator* animator)
{
	Rect d1, d2;
	Rect tmpBox = arrow->GetSprite()->GetBox();
	MovingAnimator* mov = (MovingAnimator*)arrow->GetAnimator("mov_moving");
	uint32_t currFrame = animator->GetCurrFrame();
	arrow->GetSprite()->SetFrame(currFrame);
	
	if (!clipper.Clip(tmpBox, layer->m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
	{
		mov->Stop();
		animator->Stop();
	}
	
	layer->m_Scene->GetColider().Check();
}

void ArrowFrameRangeFinish(Layer1* layer, Arrow* arrow)
{
	layer->m_Scene->GetColider().Cancel(layer->link->GetSprite(), arrow->GetSprite());
	arrow->EntityDestroy();
}


Layer1::Layer1()
	: Layer("Layer1")
{
}

void Layer1::InitializeTeleports()
{
	std::ifstream file("Assets/Config/TeleportPoints/TeleportPoints.json");
	std::ifstream efile(("Assets/Config/TeleportPoints/ElevatorTeleportPoints.json"));
	json Points = json::parse(file);
	json ePoints = json::parse(efile);
	Sprite origin, dest;
	ID id = 0;

	for (auto p : Points["points"])
	{	
		Teleport tmp;

		tmp.origin = m_Scene->CreateSprite("tp_1" + std::to_string(id), p["origin"]["x"].get<int>() * 16, p["origin"]["y"].get<int>() * 16, NONPRINTABLE, "");
		tmp.origin->SetColiderBox(32, 16);
		tmp.dest = m_Scene->CreateSprite("tp_2" + std::to_string(id), p["destination"]["x"].get<int>() * 16, p["destination"]["y"].get<int>() * 16, NONPRINTABLE, "");
		tmp.dest->SetColiderBox(32, 16);
		tmp.stage = p["stage"].get<int>();
		m_teleports.push_back(tmp);
		
		id++;
	} 
	
	id = 0;

	for (auto p : ePoints["points"])
	{
		Teleport e_tmp;

		e_tmp.origin = m_Scene->CreateSprite("etp_1" + std::to_string(id), p["origin"]["x"].get<int>() * 16, p["origin"]["y"].get<int>() * 16, NONPRINTABLE, "");
		e_tmp.origin->SetColiderBox(32, 16);
		e_tmp.dest = m_Scene->CreateSprite("etp_2" + std::to_string(id), p["destination"]["x"].get<int>() * 16, p["destination"]["y"].get<int>() * 16, NONPRINTABLE, "");
		e_tmp.dest->SetColiderBox(32, 16);
		e_tmp.stage = p["stage"].get<int>();
		m_elevator_teleports.push_back(e_tmp);
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

void Layer1::LoadSheets()
{
	m_sheets.emplace(std::make_pair("link_sheet", new AnimationSheet("link_sheet", "Assets/AnimationFilms/link-sprites.bmp")));
	m_sheets.emplace(std::make_pair("enemy_sheet", new AnimationSheet("enemy_sheet", "Assets/AnimationFilms/enemies-collectibles-sprites.bmp")));
	m_sheets.emplace(std::make_pair("door_sheet", new AnimationSheet("enemy_sheet", "Assets/AnimationFilms/door.bmp")));
	m_sheets.emplace(std::make_pair("collectible_sheet", new AnimationSheet("collectible_sheet", "Assets/AnimationFilms/collectibles.bmp")));
	m_sheets.emplace(std::make_pair("elevator_sheet", new AnimationSheet("elevator_sheet", "Assets/AnimationFilms/elevator.bmp")));
	m_sheets.emplace(std::make_pair("block_sheet", new AnimationSheet("block_sheet", "Assets/AnimationFilms/block_film.bmp")));
}

void Layer1::InitializeEnemies(GridLayer *grid) 
{
	std::ifstream wosuFile("Assets/Config/Enemies/wosu_config.json");
	json enemies = json::parse(wosuFile);
	ID i = 0;

	for (auto e : enemies["data"])
	{
		ID id = UUID::GenerateUUID();

		m_enemies.emplace(std::make_pair( i, new Wosu(i, e["lookingAt"].get<std::string>(), e["stage"].get<uint32_t>(), m_sheets["enemy_sheet"], m_Scene)));
		m_enemies.at(i)->SetMaxX(e["max_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetMinX(e["min_x"].get<uint32_t>() * 16);
	    m_enemies.at(i)->SetSprite((m_Scene->CreateSprite("Wosu" + std::to_string(id), e["spawn_pos"]["x"].get<uint32_t>() * 16, e["spawn_pos"]["y"].get<uint32_t>() * 16, m_enemies.at(i)->GetFilm("moving_" + e["lookingAt"].get<std::string>()), "E_WOSU")));
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
	
	std::ifstream botFile("Assets/Config/Enemies/bot_config.json");
	enemies = json::parse(botFile);
 
	for (auto e : enemies["data"])
	{
		ID id = UUID::GenerateUUID();

		m_enemies.emplace(std::make_pair(i, new Bot(i, e["lookingAt"].get<std::string>(), e["stage"].get<uint32_t>(), m_sheets["enemy_sheet"], m_Scene)));
		m_enemies.at(i)->SetMaxX(e["max_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetMinX(e["min_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetSprite((m_Scene->CreateSprite("Bot" + std::to_string(id), e["spawn_pos"]["x"].get<uint32_t>() * 16, e["spawn_pos"]["y"].get<uint32_t>() * 16, m_enemies.at(i)->GetFilm("moving_" + e["lookingAt"].get<std::string>()), "E_BOT")));
		m_enemies.at(i)->GetSprite()->SetColiderBox(16, 16);
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

	std::ifstream staflosFile("Assets/Config/Enemies/staflos_config.json");
	enemies = json::parse(staflosFile);

	for (auto e : enemies["data"])
	{
		ID id = UUID::GenerateUUID();

		m_enemies.emplace(std::make_pair(i, new Staflos(i, e["lookingAt"].get<std::string>(), e["stage"].get<uint32_t>(), m_sheets["enemy_sheet"], m_Scene)));
		m_enemies.at(i)->SetMaxX(e["max_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetMinX(e["min_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetSprite((m_Scene->CreateSprite("Staflos" + std::to_string(id), e["spawn_pos"]["x"].get<uint32_t>() * 16, e["spawn_pos"]["y"].get<uint32_t>() * 16, m_enemies.at(i)->GetFilm("falling"), "E_STAFLOS")));
		m_enemies.at(i)->GetSprite()->SetColiderBox(16, 32);
		m_enemies.at(i)->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), m_enemies.at(i)->GetSprite()));
		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnSolidGround([grid](Rect& r) { return grid->IsOnSolidGround(r); });

		MovingAnimator* anim = (MovingAnimator*)m_enemies.at(i)->GetAnimator("mov_gravity");
		MovingAnimation* down = (MovingAnimation*)m_enemies.at(i)->GetAnimation("mov_gravity");

		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnStartFalling([anim, down]() {
			anim->Start(down, SystemClock::GetDeltaTime());
			});

		m_enemies.at(i)->GetSprite()->GetGravityHandler().SetOnStopFalling([anim, down]() {
			anim->Stop();
			});

		i++;
	}


	std::ifstream gumaFile("Assets/Config/Enemies/guma_config.json");
	enemies = json::parse(gumaFile);

	for (auto e : enemies["data"])
	{
		ID id = UUID::GenerateUUID();

		m_enemies.emplace(std::make_pair(i, new Guma(i, e["lookingAt"].get<std::string>(), e["stage"].get<uint32_t>(), m_sheets["enemy_sheet"], m_Scene)));
		m_enemies.at(i)->SetMaxX(e["max_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetMinX(e["min_x"].get<uint32_t>() * 16);
		m_enemies.at(i)->SetSprite((m_Scene->CreateSprite("Guma" + std::to_string(id), e["spawn_pos"]["x"].get<uint32_t>() * 16, e["spawn_pos"]["y"].get<uint32_t>() * 16, m_enemies.at(i)->GetFilm("moving_"+ e["lookingAt"].get<std::string>()), "E_GUMA")));
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

		i++;
	}


}

void Layer1::CreateCollectible(std::string jsonPath, std::string type, enum c_type cType) {
	std::ifstream file(jsonPath);
	json collectible = json::parse(file);

	ID i = 0;
	std::vector<Collectible*> tmp;

	for (auto item : collectible["data"])
	{
		tmp.push_back(new Collectible(i, m_sheets["collectible_sheet"], m_Scene, cType));
		tmp.at(i)->SetSprite(m_Scene->CreateSprite(type + std::to_string(i), item["spawn_pos"]["x"].get<uint32_t>() * 16, item["spawn_pos"]["y"].get<uint32_t>() * 16, tmp.at(i)->GetFilm(type + "_film"), ""));
		tmp.at(i)->GetSprite()->SetColiderBox(16, 16);
		i++;
	}
	m_collectibles.emplace(std::make_pair(cType, tmp));

}

void Layer1::DropCollectible(Enemy* enemy) {
	ID random = UUID::GenerateUUID() % 10;
	
	if (random > 2) {
		return;
	}

	AudioManager::Get().PlaySound(m_sounds.at("itemdrop"));

	c_type cType;
	std::string type;

	switch (random) {
	case 0:	cType = C_BLUEPOTION;
			type = "bluePotion";
			break;
	case 1:	cType = C_REDPOTION;
			type = "redPotion";
			break;
	case 2:	cType = C_BASICPOINTS;
			type = "simplePointBag";
			break;
	}
	
	Collectible* tmp = new Collectible(i, m_sheets["collectible_sheet"], m_Scene, cType);
	uint32_t posY = (enemy->GetSprite()->GetTypeId() == "E_BOT") ? 0 : 16;
	tmp->SetSprite(m_Scene->CreateSprite(type +std::to_string(UUID::GenerateUUID()), enemy->GetSprite()->GetPosX(), enemy->GetSprite()->GetPosY() + posY, tmp->GetFilm(type + "_film"), ""));
	tmp->GetSprite()->SetColiderBox(16, 16);
	tmp->setCooldown(700);
	
	m_collectibles[cType].push_back(tmp);
}

void Layer1::InitialiazeCollectibles()
{
	CreateCollectible("Assets/Config/Collectibles/key_config.json", "key", C_KEY);
	
	CreateCollectible("Assets/Config/Collectibles/blue_potion_config.json", "bluePotion", C_BLUEPOTION);
	CreateCollectible("Assets/Config/Collectibles/red_potion_config.json", "redPotion", C_REDPOTION);
	CreateCollectible("Assets/Config/Collectibles/extra_life_config.json", "extraLife", C_LINK);
	//CreateCollectible("Assets/Config/Collectibles/basic_point_bag_config.json", "simplePointBag", C_BASICPOINTS);
	//CreateCollectible("Assets/Config/Collectibles/stronger_point_bag_config.json", "strongerPointBag", C_STRONGERPOINTS);
}

void Layer1::InitializeDoors()
{
	std::ifstream doorFile("Assets/Config/Doors/door_config.json");
	json doors = json::parse(doorFile);

	ID i = 0;
	
	for (auto d : doors["data"])
	{	
		ID id = UUID::GenerateUUID();
		m_doors.emplace(std::make_pair(i, new Door(i, m_sheets["door_sheet"], m_Scene)));
		m_doors.at(i)->SetSprite(m_Scene->CreateSprite("Door" + std::to_string(id), d["spawn_pos"]["x"].get<uint32_t>() * 16, d["spawn_pos"]["y"].get<uint32_t>() * 16, m_doors.at(i)->GetFilm("open_"), ""));
		m_doors.at(i)->GetSprite()->SetColiderBox(7, 48);
		i++;
	}
}

void Layer1::InitializeAudio()
{	
	m_sounds.emplace(std::make_pair("attacking", AudioManager::Get().LoadSound("Assets/Sounds/Link/attacking_sound.wav")));
	m_sounds.emplace(std::make_pair("gameover", AudioManager::Get().LoadSound("Assets/Sounds/Link/dead.wav")));
	m_sounds.emplace(std::make_pair("healing", AudioManager::Get().LoadSound("Assets/Sounds/Link/healing.wav")));
	m_sounds.emplace(std::make_pair("shield", AudioManager::Get().LoadSound("Assets/Sounds/Link/shield.wav")));
	m_sounds.emplace(std::make_pair("itemdrop", AudioManager::Get().LoadSound("Assets/Sounds/Link/item_drop.wav")));
	m_sounds.emplace(std::make_pair("door", AudioManager::Get().LoadSound("Assets/Sounds/Misc/door_opening.wav")));
	m_sounds.emplace(std::make_pair("enemy_damage", AudioManager::Get().LoadSound("Assets/Sounds/Enemies/enemy_damage.wav")));
	m_sounds.emplace(std::make_pair("key", AudioManager::Get().LoadSound("Assets/Sounds/Misc/key_collected.wav")));
	m_sounds.emplace(std::make_pair("elevator", AudioManager::Get().LoadSound("Assets/Sounds/Misc/elevator.wav")));
	m_sounds.emplace(std::make_pair("block", AudioManager::Get().LoadSound("Assets/Sounds/Misc/block_breaking.wav")));
	m_sounds.emplace(std::make_pair("link_damage", AudioManager::Get().LoadSound("Assets/Sounds/Link/receive_damage.wav")));
	m_sounds.emplace(std::make_pair("collect", AudioManager::Get().LoadSound("Assets/Sounds/Link/pickup_item.wav")));
	m_sounds.emplace(std::make_pair("jump_spell", AudioManager::Get().LoadSound("Assets/Sounds/Link/level_up.wav")));
	m_sounds.emplace(std::make_pair("respawn", AudioManager::Get().LoadSound("Assets/Sounds/Link/lost_life.wav")));
	m_sounds.emplace(std::make_pair("level_complete", AudioManager::Get().LoadSound("Assets/Sounds/Link/level_complete.wav")));
	m_sounds.emplace(std::make_pair("ultimate", AudioManager::Get().LoadSound("Assets/Sounds/Link/ulti.wav")));
}

void Layer1::InitializeElevators(GridLayer* grid)
{
	std::ifstream elevatorFile("Assets/Config/Elevator/elevator_config.json");
	json elevators = json::parse(elevatorFile);
	ID id = 0;
	for (auto i : elevators["data"])
	{
		m_elevators.emplace(std::make_pair(id, new Elevator(id, m_sheets["elevator_sheet"], m_Scene)));
		m_elevators.at(id)->SetSprite(m_Scene->CreateSprite("Elevator" + std::to_string(id), i["spawn_pos"]["x"].get<uint32_t>() * 16, i["spawn_pos"]["y"].get<uint32_t>() * 16, m_elevators.at(id)->GetFilm("elevator_film"), ""));
		m_elevators.at(id)->GetSprite()->SetColiderBox(32, 56);
		m_elevators.at(id)->GetSprite()->SetMover(MakeSpriteGridLayerMover(m_Scene->GetTiles()->GetGrid(), m_elevators.at(id)->GetSprite()));
		id++;
	}

}

void Layer1::InitializeBridge()
{
	int index = 0;
	for (int i = 471; i <= 503; i++)
	{
		m_blocks.push_back(new Block(i, m_sheets.at("block_sheet"), m_Scene));
		m_blocks[index]->SetSprite(m_Scene->CreateSprite("Block" + std::to_string(i), i * 16, 8 * 16, m_blocks[index]->GetFilm("break_"), "S_BLOCK"));
		m_blocks[index]->GetSprite()->SetColiderBox(16, 16);
		index++;
	}

	m_lava = m_Scene->CreateSprite("lava", 471 * 16, 15 * 16, NONPRINTABLE, "E_LAVA");
	m_lava->SetColiderBox(35 * 16, 16);

	m_end = m_Scene->CreateSprite("end", 953 * 16, 12 * 16, NONPRINTABLE, "E_END");
	m_end->SetColiderBox(16, 32);
}

void Layer1::ResetElevators()
{
	std::ifstream elevatorFile("Assets/Config/Elevator/elevator_config.json");
	json elevators = json::parse(elevatorFile);
	ID id = 0;
	for (auto i : elevators["data"])
	{
		m_elevators.at(id)->GetSprite()->SetPos(i["spawn_pos"]["x"].get<uint32_t>() * 16, i["spawn_pos"]["y"].get<uint32_t>() * 16);
		id++;
	}

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
		else 
		{
			spell.setDurationRemainingTime(spell.getDurationRemainingTime() - ts);

			if (spell.getDurationRemainingTime() <= 0) {
				spell.setDurationRemainingTime(0);

				if (spell.getType() == "jumpspell") {
					link->setJumpingForce(link->getJumpingForce() / 2);
					link->EraseAnimation(link->jumpAnimation);
					link->jumpAnimation = new MovingAnimation("mov_jumping", link->getJumpingForce(), 0, 0, 20);
					link->EmplaceAnimation(link->jumpAnimation);
				}

				if (spell.getType() == "shieldspell") {
					link->shieldspell.GetAnimator("frame_animator")->Stop();
				}

			}
			
			if (!link->thunderspell.isActive()) 
			{
				link->SetKritikos(false);
				if (pausedMusic)
				{
					pausedMusic = false;
					AudioManager::Get().UnPauseMusic();
				}
			}


			//ENGINE_TRACE(spell.getDurationRemainingTime());
		}
	}
}

void Layer1::UpdateCollectibleCooldowns(Time ts) {
	for (auto c : m_collectibles)
	{
		int cnt = 0;
		for (auto i : c.second)
		{
			if (i->getCooldown() > 0) {
				i->setCooldown(i->getCooldown() - ts);
			}
			if (i->getCooldown() < 0) {
				i->setCooldown(0);
			}

		}
	}
}

void Layer1::CheckTimers(Time ts) {
	UpdateSpell(link->lifespell, ts);
	UpdateSpell(link->jumpspell, ts);
	UpdateSpell(link->shieldspell, ts);
	UpdateSpell(link->thunderspell, ts);
	
	UpdateCollectibleCooldowns(ts);

	if (link->getDamageCoolDown() != 0)
		link->setDamageCoolDown(link->getDamageCoolDown() - ts);
	if (link->getDamageCoolDown() < 0)
		link->setDamageCoolDown(0);
	if (link->getDamageCoolDown() == 0 && link->GetState() == "damage_from") {

		if (!KeyboardInput::IsPressed(SCANCODE_D) && !KeyboardInput::IsPressed(SCANCODE_A))
			link->SetState("idle");
		else if (KeyboardInput::IsPressed(SCANCODE_D)) {
			link->SetState("moving");
			link->SetLookingAt("right");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"))->GetStartFrame());
		}
		else if (KeyboardInput::IsPressed(SCANCODE_A)) {
			link->SetState("moving");
			link->SetLookingAt("left");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"))->GetStartFrame());
		}
	}

	if (link->getAttackingStateCoolDown() != 0)
		link->setAttackingStateCoolDown(link->getAttackingStateCoolDown() - ts);
	if (link->getAttackingStateCoolDown() < 0)
		link->setAttackingStateCoolDown(0);
	if (link->getAttackingStateCoolDown() == 0 && link->GetState() == "attacking") {

		if(!KeyboardInput::IsPressed(SCANCODE_D) && !KeyboardInput::IsPressed(SCANCODE_A))
			link->SetState("idle");
		else if(KeyboardInput::IsPressed(SCANCODE_D)){
			link->SetState("moving");
			link->SetLookingAt("right");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"))->GetStartFrame());
		}
		else if (KeyboardInput::IsPressed(SCANCODE_A)) {
			link->SetState("moving");
			link->SetLookingAt("left");
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"))->GetStartFrame());
		}
	}


	for (auto e : m_enemies)
	{
		if (e.second->getDamageCoolDown() != 0)
			e.second->setDamageCoolDown(e.second->getDamageCoolDown() - ts);
		if (e.second->getDamageCoolDown() < 0)
			e.second->setDamageCoolDown(0);

		if (e.second->GetSprite()->GetTypeId() == "E_BOT")
		{
			Bot* tmp = (Bot*)e.second;
			if (tmp->GetJumpCooldown() != 0)
				tmp->SetJumpCooldown(tmp->GetJumpCooldown() - ts);

			if (tmp->GetJumpCooldown() < 0)
				tmp->SetJumpCooldown(0);
		}
		else if (e.second->GetSprite()->GetTypeId() == "E_STAFLOS")
		{
			Staflos* tmp = (Staflos*)e.second;
			if (tmp->GetAttackCooldown() != 0)
				tmp->SetAttackCooldown(tmp->GetAttackCooldown() - ts);

			if (tmp->GetAttackCooldown() < 0)
				tmp->SetAttackCooldown(0);

		}
		else if (e.second->GetSprite()->GetTypeId() == "E_GUMA")
		{
			Guma* tmp = (Guma*)e.second;
			if (tmp->GetAttackCooldown() != 0)
				tmp->SetAttackCooldown(tmp->GetAttackCooldown() - ts);

			if (tmp->GetAttackCooldown() < 0)
				tmp->SetAttackCooldown(0);
		}
	}

	for (auto b : m_blocks)
	{
		if (b->GetRespawnCooldown() != 0)
			b->SetRespawnCooldown(b->GetRespawnCooldown() - ts);
		
		if (b->GetRespawnCooldown() < 0)
			b->SetRespawnCooldown(0);
	}


}

void Layer1::SpellFollowLink() {
	link->lifespell.GetSprite()->SetPos(link->GetSprite()->GetPosX() - 10, link->GetSprite()->GetPosY());
	link->jumpspell.GetSprite()->SetPos(link->GetSprite()->GetPosX()-10, link->GetSprite()->GetPosY());
	link->shieldspell.GetSprite()->SetPos(link->GetSprite()->GetPosX()-8, link->GetSprite()->GetPosY());
	link->thunderspell.GetSprite()->SetPos(link->GetSprite()->GetPosX()-10, link->GetSprite()->GetPosY());
}

void Layer1::onStart()
{	
	m_Scene = MakeReference<Scene>(1);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	clipper = InitClipper(m_Scene->GetTiles().get());
	currElevator = nullptr;

	InitilizeLayer(this);

	pausedMusic = false;
	
	link = new Link(m_sheets["link_sheet"], m_Scene);
	
	link->shieldspell.SetSprite(m_Scene->CreateSprite("Shieldspell", 13 * 16, 10 * 16, link->shieldspell.GetFilm("shieldspell_"), ""));
	link->shieldspell.GetSprite()->SetZorder(2);

	link->SetSprite(m_Scene->CreateSprite("Link", 13 * 16, 10 * 16, link->GetFilm("moving_right"), ""));
	link->GetSprite()->SetColiderBox(16, 32);
	link->GetSprite()->SetZorder(1);

	link->lifespell.SetSprite(m_Scene->CreateSprite("Lifespell", 13 * 16, 10 * 16, link->lifespell.GetFilm("lifespell_"), ""));
	link->lifespell.GetSprite()->SetZorder(0);

	link->jumpspell.SetSprite(m_Scene->CreateSprite("Jumpspell", 13 * 16, 10 * 16, link->jumpspell.GetFilm("jumpspell_"), ""));
	link->jumpspell.GetSprite()->SetZorder(0);

	link->thunderspell.SetSprite(m_Scene->CreateSprite("Thunderspell", 13 * 16, 10 * 16, link->thunderspell.GetFilm("thunderspell_"), ""));
	link->thunderspell.GetSprite()->SetZorder(0);
	
	flash.SetSprite(m_Scene->CreateSprite("Flash", m_Scene->GetTiles()->GetViewWindow().x, 0, flash.GetFilm("flash_"), ""));
	flash.GetSprite()->SetZorder(0);

	Rect r = m_Scene->GetTiles()->GetViewWindow();
	r.x = link->GetSprite()->GetPosX() - (r.w / 2);
	m_Scene->GetTiles()->SetViewWindow(r);

	m_currStage = 1;
	bounds = new std::pair<int, int>;
	*bounds = m_stages.at(0);
	GridLayer* grid = m_Scene->GetTiles()->GetGrid();
	gridlayer = grid;
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
	InitializeElevators(grid);

}

void Layer1::onDelete()
{

}

void Layer1::onUpdate(Time ts)
{
	*bounds = m_stages.at(m_currStage - 1);
	TeleportHandler();
	//ArrowHandler();
	EnemyMovement();
	EnemyHandler();
	DoorHandler();
	CollectibleHandler();
	BridgeHandler();

	ElevatorHandler();
	SpellFollowLink();
	WaypointHandler();

	CheckTimers(ts);

	flash.GetSprite()->SetPos(m_Scene->GetTiles()->GetViewWindow().x, 0);
	if (waitTeleportCounter < 15) {
		waitTeleportCounter++;
	}

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySceneTiles();
	Renderer::DisplaySprites();
	//Renderer::DebugDisplayGrid();
	Renderer::EndScene();
}

void Layer1::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(Layer1::LinkStartAnimations));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(Layer1::LinkStopAnimations));
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(Layer1::ElevatorStart));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(Layer1::ElevatorStop));
}

bool Layer1::LinkStartAnimations(KeyTapEvent& e)
{
	if (e.GetKey() == InputKey::d && !KeyboardInput::IsPressed(SCANCODE_S))
	{
		FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
		if (!tmp->HasFinished())
			tmp->Stop();
		link->SetState("moving");
		link->SetLookingAt("right");
		tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_right"))->GetStartFrame());
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Start((MovingAnimation*)link->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
		
	}
		
	if (e.GetKey() == InputKey::a && !KeyboardInput::IsPressed(SCANCODE_S))
	{
		FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
		if (!tmp->HasFinished())
			tmp->Stop();
		link->SetState("moving");
		link->SetLookingAt("left");
		tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_moving_left"))->GetStartFrame());
		
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Start((MovingAnimation*)link->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
	}
	
	if (e.GetKey() == InputKey::s && 
		!KeyboardInput::IsPressed(SCANCODE_D) && 
		!KeyboardInput::IsPressed(SCANCODE_A))
	{
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
		if (link->GetLookingAt() == "right" && link->GetState() != "attacking") 
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("crouch");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_right"))->GetStartFrame());

		}
		else if (link->GetLookingAt() == "left" && link->GetState() != "attacking") 
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("crouch");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_left"))->GetStartFrame());
		}
	}

	if (e.GetKey() == InputKey::q)
	{
		link->setAttackingStateCoolDown(500);

		if (link->GetLookingAt() == "right" && link->GetState() == "crouch") 
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("crouch_attack");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_right"))->GetStartFrame());

		}
		else if (link->GetLookingAt() == "left" && link->GetState() == "crouch") 
		{
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("crouch_attack");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_crouch_attack_left"))->GetStartFrame());
			AudioManager::Get().PlaySound(m_sounds.at("attacking"));
		}
		else if (link->GetLookingAt() == "right" && link->GetState() != "attacking") {
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("attacking");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_right"))->GetStartFrame());
			
		}
		else if (link->GetLookingAt() == "left" && link->GetState() != "attacking") {
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("attacking");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_attacking_left"))->GetStartFrame());

			
		}
		AudioManager::Get().PlaySound(m_sounds.at("attacking"));
	}

	if (e.GetKey() == InputKey::SPACE)
	{
		MovingAnimator* tmp = (MovingAnimator*)link->GetAnimator("mov_jumping");
		tmp->Start((MovingAnimation*)link->GetAnimation("mov_jumping"), SystemClock::GetDeltaTime());
		link->SetState("jumping");
	}

	if (e.GetKey() == InputKey::NUM_1)
	{

		if (link->getMagicPoints() >= link->lifespell.getCost() && link->lifespell.canUse()) {
			link->setMagicPoints(link->getMagicPoints() - link->lifespell.getCost());
			link->heal(50);
			link->lifespell.setDurationRemainingTime(link->lifespell.getDuration());
			link->lifespell.setCooldownRemainingTime(link->lifespell.getCooldown());
			AudioManager::Get().PlaySound(m_sounds.at("healing"));
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->lifespell.GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)link->lifespell.GetAnimation("lifespell_animation"), SystemClock::GetDeltaTime(),
				((FrameRangeAnimation*)link->lifespell.GetAnimation("lifespell_animation"))->GetStartFrame());

		}
	}

	if (e.GetKey() == InputKey::NUM_2)
	{

		if (link->getMagicPoints() >= link->jumpspell.getCost() && link->jumpspell.canUse()) {
			link->setMagicPoints(link->getMagicPoints() - link->jumpspell.getCost());
			link->setJumpingForce(link->getJumpingForce() * 2);
			link->jumpspell.setDurationRemainingTime(link->jumpspell.getDuration());
			link->jumpspell.setCooldownRemainingTime(link->jumpspell.getCooldown());

			link->EraseAnimation(link->jumpAnimation);
			
			link->jumpAnimation = new MovingAnimation("mov_jumping", link->getJumpingForce(), 0, 0, 12);
			link->EmplaceAnimation(link->jumpAnimation);
			
			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->jumpspell.GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)link->jumpspell.GetAnimation("jumpspell_animation"), SystemClock::GetDeltaTime(),
				((FrameRangeAnimation*)link->jumpspell.GetAnimation("jumpspell_animation"))->GetStartFrame());

			AudioManager::Get().PlaySound(m_sounds.at("jump_spell"));
		}

	}
		
	if (e.GetKey() == InputKey::NUM_3)
	{
		if (link->getMagicPoints() >= link->shieldspell.getCost() && link->shieldspell.canUse()) {
			link->setMagicPoints(link->getMagicPoints() - link->shieldspell.getCost());
			link->shieldspell.setDurationRemainingTime(link->shieldspell.getDuration());
			link->shieldspell.setCooldownRemainingTime(link->shieldspell.getCooldown());
			AudioManager::Get().PlaySound(m_sounds.at("shield"));

			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->shieldspell.GetAnimator("frame_animator");
			tmp->Start((FrameRangeAnimation*)link->shieldspell.GetAnimation("shieldspell_animation"), SystemClock::GetDeltaTime(),
				((FrameRangeAnimation*)link->shieldspell.GetAnimation("shieldspell_animation"))->GetStartFrame());
		}
	}
		
	if (e.GetKey() == InputKey::NUM_4)
	{
		if (link->getMagicPoints() >= link->thunderspell.getCost() && link->thunderspell.canUse()) {
			link->setMagicPoints(link->getMagicPoints() - link->thunderspell.getCost());
			link->thunderspell.setDurationRemainingTime(link->thunderspell.getDuration());
			link->thunderspell.setCooldownRemainingTime(link->thunderspell.getCooldown());

			FrameRangeAnimator* tmp = (FrameRangeAnimator*)link->thunderspell.GetAnimator("frame_animator");
				tmp->Start((FrameRangeAnimation*)link->thunderspell.GetAnimation("thunderspell_animation"), SystemClock::GetDeltaTime(),
				((FrameRangeAnimation*)link->thunderspell.GetAnimation("thunderspell_animation"))->GetStartFrame());
			
			link->SetKritikos(true);
			tmp = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
			if (!tmp->HasFinished())
				tmp->Stop();
			link->SetState("voskos");
			tmp->Start((FrameRangeAnimation*)link->GetAnimation("frame_voskos_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_voskos_" + link->GetLookingAt()))->GetStartFrame());
	
			AudioManager::Get().PlaySound(m_sounds.at("ultimate"));
			AudioManager::Get().PauseMusic();
			pausedMusic = true;
		}
	}


	return true;
}

bool Layer1::LinkStopAnimations(KeyReleaseEvent& e)
{
	if (e.GetKey() == InputKey::d)
	{
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
		((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();
		link->SetState("idle");
	}
	
	if (e.GetKey() == InputKey::a)
	{
		((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
		((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();
		link->SetState("idle");
	}
	
	if (e.GetKey() == InputKey::s)
	{
		if (!((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->HasFinished())
			((FrameRangeAnimator*)link->GetAnimator("frame_animator"))->Stop();

		if (link->GetLookingAt() == "right")
		{
			if (link->IsKritikos())
				link->GetSprite()->SetFilm(link->GetFilm("kritiko_moving_right"));
			else
				link->GetSprite()->SetFilm(link->GetFilm("moving_right"));
		}
		else if (link->GetLookingAt() == "left")
		{
			if (link->IsKritikos())
				link->GetSprite()->SetFilm(link->GetFilm("kritiko_moving_left"));
			else
				link->GetSprite()->SetFilm(link->GetFilm("moving_left"));
		}

		link->GetSprite()->SetFrame(0);
		link->SetState("idle");
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
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)flash.GetAnimator("frame_animator");
				if (tmp->HasFinished()) 
				{
					waitTeleportCounter = 0;
					tmp->Start((FrameRangeAnimation*)flash.GetAnimation("flash_animation"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)flash.GetAnimation("flash_animation"))->GetStartFrame());
				}

				if (waitTeleportCounter == 12) {
					link_sprite->SetPos(dest->GetPosX(), dest->GetPosY());
					m_currStage = i.stage;
					int32_t dx = dest->GetPosX() - tilelayer->GetViewWindow().x - (tilelayer->GetViewWindow().w / 2);

					if (tilelayer->CanScrollHoriz(dx))
						tilelayer->Scroll(dx, 0);
				}
				
			});
			
			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.origin);
		}
	}
	
	if (!currElevator)
		return;

	Sprite el_sprite = currElevator->GetSprite();
	for (auto i : m_elevator_teleports)
	{
		tmpBox = i.origin->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{
			Sprite dest = i.dest;
			m_Scene->GetColider().Register(el_sprite, i.origin, [el_sprite, link_sprite, dest, tilelayer, this, i](Sprite s1, Sprite s2) {
				FrameRangeAnimator* tmp = (FrameRangeAnimator*)flash.GetAnimator("frame_animator");
				if (tmp->HasFinished())
				{
					waitTeleportCounter = 0;
					tmp->Start((FrameRangeAnimation*)flash.GetAnimation("flash_animation"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)flash.GetAnimation("flash_animation"))->GetStartFrame());
				}
				ENGINE_TRACE(waitTeleportCounter);

				if (waitTeleportCounter == 15) {

					el_sprite->SetPos(dest->GetPosX(), dest->GetPosY());
					link_sprite->SetPos(dest->GetPosX() + 8, dest->GetPosY() + 16);
					m_currStage = i.stage;
					int32_t dx = dest->GetPosX() - tilelayer->GetViewWindow().x - (tilelayer->GetViewWindow().w / 2);
					if (tilelayer->CanScrollHoriz(dx))
						tilelayer->Scroll(dx, 0);
				}
			});

			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(el_sprite, i.origin);
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
		
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{
			if (e.second->GetState() != "death")
			{
				e.second->SetState("moving");

				if (e.second->GetSprite()->GetTypeId() == "E_WOSU")
				{
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

					if (mov->HasFinished())
						mov->Start((MovingAnimation*)e.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());

					if (anim->HasFinished())
						anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()))->GetStartFrame());

				}
				else if (e.second->GetSprite()->GetTypeId() == "E_STAFLOS")
				{
					Staflos* tmp_staflos = (Staflos*)e.second;
					if (link->GetSprite()->GetPosX() < e.second->GetSprite()->GetPosX() && e.second->GetLookingAt() != "left")
					{
						e.second->SetLookingAt("left");
						anim->Stop();
					}
					else if (link->GetSprite()->GetPosX() > e.second->GetSprite()->GetPosX() && e.second->GetLookingAt() != "right")
					{
						e.second->SetLookingAt("right");
						anim->Stop();
					}
					
					if (!tmp_staflos->isSleeping())
					{
						if (tmp_staflos->GetAttackCooldown() == 0)
						{
							ENGINE_TRACE("STAFLOS_ATTACK");
							tmp_staflos->SetAttackCooldown(2000);
							anim->Stop();
							e.second->SetState("attacking");
							anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_attacking_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_attacking_" + e.second->GetLookingAt()))->GetStartFrame());
						}

						if (anim->HasFinished())
							anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()))->GetStartFrame());
						
						if (mov->HasFinished())
							mov->Start((MovingAnimation*)e.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
					}
					else
					{
						int32_t dx = 0;
						dx = link->GetSprite()->GetPosX() - tmp_staflos->GetSprite()->GetPosX();
						if (std::abs(dx) < 96)
						{
							tmp_staflos->GetSprite()->GetGravityHandler().SetGravityAddicted(true);
							tmp_staflos->SetSleeping(false);
						}

					}

				}
				else if (e.second->GetSprite()->GetTypeId() == "E_BOT")
				{
					Bot* tmp_bot = (Bot*)e.second;
					MovingAnimator* jump = (MovingAnimator*)tmp_bot->GetAnimator("mov_jumping");

					if (link->GetSprite()->GetPosX() < tmp_bot->GetSprite()->GetPosX())
						tmp_bot->SetLookingAt("left");
					else if (link->GetSprite()->GetPosX() > tmp_bot->GetSprite()->GetPosX())
						tmp_bot->SetLookingAt("right");

					if (anim->HasFinished())
						anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()))->GetStartFrame());


					if(mov->HasFinished())
						mov->Start((MovingAnimation*)e.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
					if (jump->HasFinished() && tmp_bot->GetJumpCooldown() == 0) {
						jump->Start((MovingAnimation*)e.second->GetAnimation("mov_jumping"), SystemClock::GetDeltaTime());
						tmp_bot->SetJumpCooldown(1000);
					}
	
				}
				else if (e.second->GetSprite()->GetTypeId() == "E_GUMA")
				{
					Guma* tmp_guma = (Guma*)e.second;
					if (e.second->GetSprite()->GetPosX() > e.second->GetMaxX())
					{
						e.second->SetLookingAt("left");
					}
					else if (e.second->GetSprite()->GetPosX() < e.second->GetMinX())
					{
						e.second->SetLookingAt("right");
					}

					if (anim->HasFinished())
						anim->Start((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)e.second->GetAnimation("frame_moving_" + e.second->GetLookingAt()))->GetStartFrame());

					if (mov->HasFinished())
						mov->Start((MovingAnimation*)e.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());

					if (tmp_guma->GetAttackCooldown() == 0)
					{
						ENGINE_TRACE("GUMA ATTACK");
						tmp_guma->SetAttackCooldown(1000);
						SpawnArrow(e.second->GetLookingAt(), e.second->GetSprite()->GetPosX(), e.second->GetSprite()->GetPosY());
					}
				}

				
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
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2) && link->getDamageCoolDown() == 0 && !link->isInvisible())
		{
			m_Scene->GetColider().Register(link_sprite, i.second->GetSprite(), [link_sprite, this, i](Sprite s1, Sprite s2) {
				int32_t dx = (link->GetLookingAt() == "right") ? -16 : 16;
				FrameRangeAnimator* anim = (FrameRangeAnimator*)link->GetAnimator("frame_animator");
				MovingAnimator* mov = (MovingAnimator*)link->GetAnimator("mov_damage");

				if (link->GetState() != "attacking" && link->GetState() != "crouch_attack")
				{

					link->setDamageCoolDown(500);

					if (link->GetState() == "crouch")
					{
						if (link->GetLookingAt() == i.second->GetLookingAt())
						{
							link->SetState("damage_from");

							if (!anim->HasFinished())
								anim->Stop();
							
							if (!mov->HasFinished())
								mov->Stop();
							
							AudioManager::Get().PlaySound(m_sounds.at("link_damage"));
							anim->Start((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()))->GetStartFrame());
							link->takeDamage(i.second->GetDamage());
						}
						else
						{
							uint32_t posX;
							if (i.second->GetLookingAt() == "right")
							{
								posX = link->GetSprite()->GetPosX() - 16;
							}
							else if (i.second->GetLookingAt() == "left")
							{
								posX = link->GetSprite()->GetPosX() + 16;
							}
							i.second->GetSprite()->SetPos(posX, i.second->GetSprite()->GetPosY());
						}

					}
					else
					{
						link->SetState("damage_from");

						if (!anim->HasFinished())
							anim->Stop();

						if (!mov->HasFinished())
							mov->Stop();

						AudioManager::Get().PlaySound(m_sounds.at("link_damage"));
						anim->Start((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)link->GetAnimation("frame_damage_from_" + link->GetLookingAt()))->GetStartFrame());
						link->takeDamage(i.second->GetDamage());
					}

					if (mov->HasFinished())
						mov->Start((MovingAnimation*)link->GetAnimation("mov_damage"), SystemClock::GetDeltaTime());

					if (link->getHealth() <= 0) 
					{
						link->setHealth(link->getMaxHealth());

						if (link->getLives() == 0) {
							AudioManager::Get().PlaySound(m_sounds.at("gameover"));
							AudioManager::Get().PauseMusic();
							Application::Instance().Freeze(); 
						}
						else 
						{
							AudioManager::Get().PlaySound(m_sounds.at("respawn"));
							ResetElevators();
							ENGINE_TRACE(m_currStage);
							if (m_currStage == 3 || m_currStage == 4) {
								link->GetSprite()->SetPos(262 * 16, 12 * 16);
								int32_t dx = (262 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
								if (m_Scene->GetTiles()->CanScrollHoriz(dx))
									m_Scene->GetTiles()->Scroll(dx, 0);
								m_currStage = 2;
							}
							else if (m_currStage == 5 || m_currStage == 6 || m_currStage == 7 || m_currStage == 8) {
								link->GetSprite()->SetPos(351 * 16, 12 * 16);
								int32_t dx = (351 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
								if (m_Scene->GetTiles()->CanScrollHoriz(dx))
									m_Scene->GetTiles()->Scroll(dx, 0);
								m_currStage = 2;

							}
							else {
								link->GetSprite()->SetPos(13 * 16, 10 * 16);
								int32_t dx = (13 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
								if (m_Scene->GetTiles()->CanScrollHoriz(dx))
									m_Scene->GetTiles()->Scroll(dx, 0);
								m_currStage = 1;
							}
						}
						
					}

					if (link->getHealth() <= 0) {
						link->setHealth(link->getMaxHealth());

						link->GetSprite()->SetPos(16 * 16, 10 * 16);
						AudioManager::Get().PlaySound(m_sounds.at("lost_life"));
						int32_t dx = (16 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
						m_currStage = 1;
						if (m_Scene->GetTiles()->CanScrollHoriz(dx))
							m_Scene->GetTiles()->Scroll(dx, 0);
					}
				}
				else
				{
					ENGINE_TRACE(i.second->GetHealth());

					if (i.second->GetSprite()->GetTypeId() != "E_STAFLOS" && i.second->getDamageCoolDown() == 0)
					{
						i.second->TakeDamage(link->getDamage());
						AudioManager::Get().PlaySound(m_sounds.at("enemy_damage"));
						i.second->setDamageCoolDown(200);
					}
					else if (link->GetState() == "crouch_attack" && i.second->getDamageCoolDown() == 0)
					{
						i.second->TakeDamage(link->getDamage());
						AudioManager::Get().PlaySound(m_sounds.at("enemy_damage"));
						i.second->setDamageCoolDown(200);
					}

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

	if (dying) {
		DropCollectible(dying);
		m_enemies.erase(m_enemies.find(dying->GetID()));
	}

}

void Layer1::DoorHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_doors.begin()->second->GetSprite()->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	Door* opened = nullptr;

	for (auto i : m_doors)
	{
		tmpBox = i.second->GetSprite()->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2) && i.second->GetState() != "open")
		{
			m_Scene->GetColider().Register(link_sprite, i.second->GetSprite(), [link_sprite, this, i](Sprite s1, Sprite s2) {

				if (link->HasKey())
				{
					link->RemoveKey();
					FrameRangeAnimator * anim = (FrameRangeAnimator*)i.second->GetAnimator("frame_animator");

					if (!anim->HasFinished())
						anim->Stop();

					i.second->SetState("open");
					AudioManager::Get().PlaySound(m_sounds.at("door"));
					anim->Start((FrameRangeAnimation*)i.second->GetAnimation("frame_open"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)i.second->GetAnimation("frame_open"))->GetStartFrame());
				}

				((MovingAnimator*)link->GetAnimator("mov_moving"))->Stop();
				if (link->GetLookingAt() == "left")
				{
					link->GetSprite()->Move(+2, 0);
				}
				else if (link->GetLookingAt() == "right")
				{
					link->GetSprite()->Move(-2, 0);
				}
			});

			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.second->GetSprite());
		}

		if (i.second->GetState() == "open")
			opened = i.second;
	}

	if (opened)
	{
		FrameRangeAnimator* anim = (FrameRangeAnimator*)opened->GetAnimator("frame_animator");
		if (anim->HasFinished())
		{
			opened->EntityDestroy();
			m_doors.erase(m_doors.find(opened->GetID()));
		}
	}
	
}

void Layer1::CollectibleHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox;
    Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	int collected_index = m_collectibles.begin()->second.size();
	Collectible* collected = nullptr;

	for (auto c : m_collectibles)
	{
		int cnt = 0;
		for (auto i : c.second)
		{
			if (i->getCooldown() > 0 ) {
				continue;
			}

			tmpBox = i->GetSprite()->GetBox();
			if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2) && (link->GetState() == "attacking") || (link->GetState() == "crouch_attack"))
			{
				m_Scene->GetColider().Register(link_sprite, i->GetSprite(), [link_sprite, this, i, c](Sprite s1, Sprite s2) {
					switch (i->GetType())
					{
					case C_KEY: link->AddKey();
								i->SetState("collected");
								AudioManager::Get().PlaySound(m_sounds.at("key"));
								break;

					case C_REDPOTION: link->setMagicPoints(link->getMagicPoints() + link->redpotion.getValue());
									  i->SetState("collected");
									  AudioManager::Get().PlaySound(m_sounds.at("collect"));
									  break;

					case C_BLUEPOTION:  link->setMagicPoints(link->getMagicPoints() + link->bluepotion.getValue());
										i->SetState("collected");
										AudioManager::Get().PlaySound(m_sounds.at("collect"));
										break;
					
					case C_LINK: link->setLives(link->getLives() + link->extralife.getValue());
								 i->SetState("collected");
								 AudioManager::Get().PlaySound(m_sounds.at("collect"));
								 break;

					case C_BASICPOINTS:  link->setPoints(link->getPoints() + link->basicpointbag.getValue());
										 i->SetState("collected");
										 AudioManager::Get().PlaySound(m_sounds.at("collect"));
										 break;

					case C_STRONGERPOINTS:  link->setPoints(link->getPoints() + link->strongerpointbag.getValue());
											i->SetState("collected");
											AudioManager::Get().PlaySound(m_sounds.at("collect"));
											break;
					}
				});

				m_Scene->GetColider().Check();
				m_Scene->GetColider().Cancel(link_sprite, i->GetSprite());
			}

			if (i->GetState() == "collected") {
				collected = i;
				collected_index = cnt;
			}
			cnt++;
		}
	}

	if (collected) {
		collected->EntityDestroy();
		m_collectibles.at(collected->GetType()).erase(m_collectibles.at(collected->GetType()).begin() + collected_index);
	}

}

bool Layer1::ElevatorStart(KeyTapEvent& e)
{
	if (!currElevator)
		return true;

	if (e.GetKey() == InputKey::DOWN)
	{
		currElevator->SetLookingAt("down");
		currElevator->SetState("moving");
		((MovingAnimator*)currElevator->GetAnimator("mov_moving"))->Start((MovingAnimation*)currElevator->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
	}
	else if (e.GetKey() == InputKey::UP)
	{
		currElevator->SetLookingAt("up");
		currElevator->SetState("moving");
		((MovingAnimator*)currElevator->GetAnimator("mov_moving"))->Start((MovingAnimation*)currElevator->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
	}

	return true;
}

bool Layer1::ElevatorStop(KeyReleaseEvent& e)
{
	if (!currElevator)
		return true;

	if (e.GetKey() == InputKey::DOWN)
	{
		((MovingAnimator*)currElevator->GetAnimator("mov_moving"))->Stop();
		currElevator->SetState("idle");
	}
	else if (e.GetKey() == InputKey::UP)
	{
		((MovingAnimator*)currElevator->GetAnimator("mov_moving"))->Stop();
		currElevator->SetState("idle");
	}

	return true;
}

void Layer1::ElevatorHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox = m_elevators.begin()->second->GetSprite()->GetBox();
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();
	
	Elevator* tmp = nullptr;

	for (auto i : m_elevators)
	{
		tmpBox = i.second->GetSprite()->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{
			m_Scene->GetColider().Register(link_sprite, i.second->GetSprite(), [link_sprite, this, i, tmp](Sprite s1, Sprite s2) {
				i.second->SetState("Selected");
				link_sprite->SetPos(link_sprite->GetPosX(), i.second->GetSprite()->GetPosY() + 16);
			});
			i.second->SetState("idle");
			m_Scene->GetColider().Check();
			m_Scene->GetColider().Cancel(link_sprite, i.second->GetSprite());
		}
		
		if (i.second->GetState() == "Selected")
			tmp = i.second;
	}

	currElevator = tmp;
	
}

void Layer1::BridgeHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox;
	Sprite link_sprite = link->GetSprite();
	TileLayer* tilelayer = m_Scene->GetTiles().get();

	for (auto i : m_blocks)
	{
		tmpBox = i->GetSprite()->GetBox();
		if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
		{
			if (i->GetState() != "dead")
			{
				m_Scene->GetColider().Register(link_sprite, i->GetSprite(), [link_sprite, this, i](Sprite s1, Sprite s2) {
					link_sprite->SetPos(link_sprite->GetPosX(), i->GetSprite()->GetPosY() - 32);
					i->SetState("break");
					FrameRangeAnimator* anim = (FrameRangeAnimator*)i->GetAnimator("frame_animator");
					if (anim->HasFinished())
					{
						anim->Start((FrameRangeAnimation*)i->GetAnimation("frame_breaking"), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)i->GetAnimation("frame_breaking"))->GetStartFrame());
						i->SetRespawnCooldown(3000);
						AudioManager::Get().PlaySound(m_sounds.at("block"));
					}
					});

				m_Scene->GetColider().Check();
				m_Scene->GetColider().Cancel(link_sprite, i->GetSprite());
			}
		}
		
		if (i->GetRespawnCooldown() == 0)
		{
			i->GetSprite()->SetFrame(0);
			i->SetState("idle");
		}
	}

}

void Layer1::SpawnArrow(std::string direction, uint32_t x, uint32_t y)
{
	uint32_t posX = 0;

	if (direction == "right")
	{
		posX = x + 16;
	}
	else if (direction == "left")
	{
		posX = x - 16;
	}	

	ID id = UUID::GenerateUUID();
	Arrow* tmp = new Arrow(id, m_sheets.at("enemy_sheet"), m_Scene);
	tmp->SetSprite(m_Scene->CreateSprite("Arrow" + std::to_string(id), posX, y, tmp->GetFilm("arrow_" + direction), "W_ARROW"));
	tmp->GetSprite()->SetColiderBox(16, 16);
	tmp->GetSprite()->SetMover(MakeSpriteGridLayerMover(gridlayer, tmp->GetSprite()));
	tmp->SetState("arrow");
	tmp->SetLookingAt(direction);
	
	MovingAnimator* mov = (MovingAnimator*)tmp->GetAnimator("mov_moving");
	FrameRangeAnimator* anim = (FrameRangeAnimator*)tmp->GetAnimator("frame_animator");

	anim->SetOnAction([this, tmp](Animator* animator, const Animation& anim) { return ArrowFrameRangeAction(this, tmp, (FrameRangeAnimator*)animator); });
	anim->SetOnStart([this, tmp](Animator* animator) { return ArrowFrameRangeStart(this, tmp); });
	anim->SetOnFinish([this, tmp](Animator* animator) { return ArrowFrameRangeFinish(this, tmp); });

	if (anim->HasFinished())
		anim->Start((FrameRangeAnimation*)tmp->GetAnimation("frame_arrow_" + direction), SystemClock::GetDeltaTime(), ((FrameRangeAnimation*)tmp->GetAnimation("frame_arrow_" + direction))->GetStartFrame());

	if (mov->HasFinished())
		mov->Start((MovingAnimation*)tmp->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
}

void Layer1::ArrowHandler()
{

}

void Layer1::WaypointHandler()
{
	Rect d1, d2;
	Rect tmpBox = m_lava->GetBox();
	Sprite link_sprite = link->GetSprite();
	if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
	{
		m_Scene->GetColider().Register(link_sprite, m_lava, [this](Sprite s1, Sprite s2) {
			link->takeDamage(20);
			ENGINE_TRACE("KAIGOMAI");

			if (link->getLives() == 0) {
				AudioManager::Get().PlaySound(m_sounds.at("gameover"));
				AudioManager::Get().PauseMusic();
				Application::Instance().Freeze();
			}
			else
			{
				AudioManager::Get().PlaySound(m_sounds.at("respawn"));
				ResetElevators();
				ENGINE_TRACE(m_currStage);
				if (m_currStage == 3 || m_currStage == 4) {
					link->GetSprite()->SetPos(262 * 16, 12 * 16);
					int32_t dx = (262 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
					if (m_Scene->GetTiles()->CanScrollHoriz(dx))
						m_Scene->GetTiles()->Scroll(dx, 0);
					m_currStage = 2;
				}
				else if (m_currStage == 5 || m_currStage == 6 || m_currStage == 7 || m_currStage == 8) {
					link->GetSprite()->SetPos(351 * 16, 12 * 16);
					int32_t dx = (351 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
					if (m_Scene->GetTiles()->CanScrollHoriz(dx))
						m_Scene->GetTiles()->Scroll(dx, 0);
					m_currStage = 2;

				}
				else {
					link->GetSprite()->SetPos(13 * 16, 10 * 16);
					int32_t dx = (13 * 16) - m_Scene->GetTiles()->GetViewWindow().x - (m_Scene->GetTiles()->GetViewWindow().w / 2);
					if (m_Scene->GetTiles()->CanScrollHoriz(dx))
						m_Scene->GetTiles()->Scroll(dx, 0);
					m_currStage = 1;
				}
			}
		});
		m_Scene->GetColider().Check();
		m_Scene->GetColider().Cancel(link_sprite, m_lava);
	}
	
	tmpBox = m_end->GetBox();
	if (clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
	{
		m_Scene->GetColider().Register(link_sprite, m_end, [this](Sprite s1, Sprite s2) {
			AudioManager::Get().PlaySound(m_sounds.at("level_complete"));
				AudioManager::Get().PauseMusic();
				Application::Instance().Freeze();
		});
		m_Scene->GetColider().Check();
		m_Scene->GetColider().Cancel(link_sprite, m_end);
	}

}
