#include "Layer0.h"

#define THE_GREATES_MPAMPIS "Mpampis"
Clipper clipper0;
int32_t cloudTimer;

const Clipper Clipper0(TileLayer* layer)
{
	return Clipper().SetView(
		[layer](void) -> const Rect&
		{ return layer->GetViewWindow(); }
	);
}

SpriteClass::Mover MakeCloudMover(Sprite sprite)
{
	return [sprite](Rect& r, int* dx, int* dy) {
		sprite->SetHasDirectMotion(true).Move(*dx, 0).SetHasDirectMotion(false);
	};
}

Layer0::Layer0()
	: Layer("Layer0")
{ 
}

void Layer0::InitializeCloudData()
{
	std::ifstream cloudFile("Assets/Config/Clouds/clouds_config.json");
	json clouds = json::parse(cloudFile);
	
	for (auto i : clouds["speeds"])
		m_speeds.push_back(i);

	for (auto i : clouds["y"])
		m_cloudY.push_back(i);
}

void Layer0::InitializeClouds()
{
	ID id;
	for (int i = 0; i < 3; i++)
	{
		id = UUID::GenerateUUID();
		int num = id % 2 + 1;
		int speed = id % 3;
		int y = id % 4;
		uint32_t posX = m_Scene->GetTiles()->GetViewWindow().x + m_Scene->GetTiles()->GetViewWindow().w - (2 * 16);
		uint32_t posY = m_cloudY[y] * 16;
		m_clouds.emplace(id, new Cloud(id, m_sheets.at("cloud_" + std::to_string(num)), m_Scene, num, m_speeds[speed]));
		m_clouds.at(id)->SetSprite(m_Scene->CreateSprite("cloud" + std::to_string(id), posX, posY, m_clouds.at(id)->GetFilm("cloud_film"), ""));
		m_clouds.at(id)->GetSprite()->SetMover(MakeCloudMover(m_clouds.at(id)->GetSprite()));
	}
	cloudTimer = 3000;
}

void Layer0::SpawnCloud()
{
	ID id = UUID::GenerateUUID();
	int num = id % 2 + 1;
	int speed = id % 3;
	int y = id % 4;
	uint32_t posX = m_Scene->GetTiles()->GetViewWindow().x + m_Scene->GetTiles()->GetViewWindow().w - 16;
	uint32_t posY = m_cloudY[y] * 16;
	m_clouds.emplace(id, new Cloud(id, m_sheets.at("cloud_" + std::to_string(num)), m_Scene, num, m_speeds.at(speed)));
	m_clouds.at(id)->SetSprite(m_Scene->CreateSprite("cloud" + std::to_string(id), posX, posY, m_clouds.at(id)->GetFilm("cloud_film"), ""));
	m_clouds.at(id)->GetSprite()->SetMover(MakeCloudMover(m_clouds.at(id)->GetSprite()));

	cloudTimer = 3000;
}

void Layer0::onStart()
{
	m_sheets.emplace("cloud_1", new AnimationSheet("cloud_0", "Assets/AnimationFilms/cloud1.bmp"));
	m_sheets.emplace("cloud_2", new AnimationSheet("cloud_1", "Assets/AnimationFilms/cloud2.bmp"));


	m_Scene = MakeReference<Scene>(0);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	clipper0 = Clipper0(m_Scene->GetTiles().get());
	m_camera = new Camera(m_Scene);

	InitializeCloudData();
	InitializeClouds();

	AudioManager::Get().PlayMusic("Assets/Sounds/Background/background_music.wav", 3);


}

void Layer0::onDelete()
{ 
}


void Layer0::onUpdate(Time ts)
{  
	if (cloudTimer != 0)
		cloudTimer -= ts;

	if (cloudTimer < 0)
		cloudTimer = 0;

	if (cloudTimer == 0)
		SpawnCloud();

	for (auto c : m_clouds)
	{	
		MovingAnimator* tmp = ((MovingAnimator*)c.second->GetAnimator("mov_moving"));
		if (tmp->HasFinished())
			tmp->Start((MovingAnimation*)c.second->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
	}

	CloudHandler();

	Renderer::BeginScene(m_Scene); 
	Renderer::DisplaySceneTiles();
	Renderer::DisplaySprites();
	//Renderer::DebugDisplayGrid();
	Renderer::EndScene();
}

void Layer0::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyTapEvent>(APP_EVENT_FUNTION(Layer0::MoveSky));
	dispatcher.Dispatch<KeyReleaseEvent>(APP_EVENT_FUNTION(Layer0::StopSky));
}

bool Layer0::MoveSky(KeyTapEvent& e)
{
	MovingAnimator* mov = m_camera->GetAnimator();
	if (e.GetKey() == InputKey::d)
	{
		m_camera->SetDirection("right");
	}
	else if (e.GetKey() == InputKey::a)
	{
		m_camera->SetDirection("left");
	}

	if (!mov->HasFinished())
		mov->Stop();
	mov->Start(m_camera->GetAnimation(), SystemClock::GetDeltaTime());

	return true;
}

bool Layer0::StopSky(KeyReleaseEvent& e)
{
	if (!m_camera->GetAnimator()->HasFinished())
		m_camera->GetAnimator()->Stop();

	return true;
}

void Layer0::CloudHandler()
{
	Rect d1;
	Rect d2;
	Rect tmpBox;

	Cloud* clipped = nullptr;

	for (auto c : m_clouds)
	{
		tmpBox = c.second->GetSprite()->GetBox();
		if (!clipper0.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
			clipped = c.second;
	}

	if (clipped)
	{
		((MovingAnimator*)clipped->GetAnimator("mov_moving"))->Stop();
		m_clouds.erase(m_clouds.find(clipped->GetID()));
		clipped->EntityDestroy();
	}
}
