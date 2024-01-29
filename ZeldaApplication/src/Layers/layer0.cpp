#include "Layer0.h"

#define THE_GREATES_MPAMPIS "Mpampis"

uint32_t cloudTimer;
//Clipper clipper;
//
//const Clipper InitClipper(TileLayer* layer)
//{
//	return Clipper().SetView(
//		[layer](void) -> const Rect&
//		{ return layer->GetViewWindow(); }
//	);
//}

Layer0::Layer0()
	: Layer("Layer0")
{ 
}

void Layer0::InitializeCloudData()
{
	std::ifstream cloudFile("Assets/Config/Clouds/cloud_config.json");
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
		int num = id % 2;
		int speed = id % 3;
		int y = id % 6;
		uint32_t posX = m_Scene->GetTiles()->GetViewWindow().x + m_Scene->GetTiles()->GetViewWindow().w - (2 * 16);
		uint32_t posY = m_cloudY[y] * 16;
		m_clouds.emplace(id, new Cloud(id, m_sheets.at("cloud_" + std::to_string(num)), m_Scene, num, speed));
		m_clouds.at(id)->SetSprite(m_Scene->CreateSprite("cloud" + std::to_string(id), posX, posY, m_clouds.at(id)->GetFilm("cloud_film"), ""));

		((MovingAnimator*)m_clouds.at(id)->GetAnimator("mov_moving"))->Start((MovingAnimation*)m_clouds.at(id)->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());
	}

	cloudTimer = 2000;
}

void Layer0::SpawnCloud()
{
	ID id = UUID::GenerateUUID();
	int num = id % 2;
	int speed = id % 3;
	int y = id % 6;
	id = UUID::GenerateUUID();
	uint32_t posX = m_Scene->GetTiles()->GetViewWindow().x + m_Scene->GetTiles()->GetViewWindow().w - (2 * 16);
	uint32_t posY = m_cloudY[y] * 16;
	m_clouds.emplace(id, new Cloud(id, m_sheets.at("cloud_" + std::to_string(num)), m_Scene, num, speed));
	m_clouds.at(id)->SetSprite(m_Scene->CreateSprite("cloud" + std::to_string(id), posX, posY, m_clouds.at(id)->GetFilm("cloud_film"), ""));

	((MovingAnimator*)m_clouds.at(id)->GetAnimator("mov_moving"))->Start((MovingAnimation*)m_clouds.at(id)->GetAnimation("mov_moving"), SystemClock::GetDeltaTime());

}

void Layer0::onStart()
{
	m_sheets.emplace("cloud_0", new AnimationSheet("cloud_0", "Assets/AnimationFilms/cloud_1.bmp"));
	m_sheets.emplace("cloud_1", new AnimationSheet("cloud_1", "Assets/AnimationFilms/cloud_2.bmp"));

	cloudTimer = 0;
	m_Scene = MakeReference<Scene>(0);
	m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	//clipper = InitClipper(m_Scene->GetTiles().get());

	m_CamLeft = MakeReference<MovingAnimator>();
	m_CamRight = MakeReference <MovingAnimator>();
}

void Layer0::onDelete()
{ 
}

void Layer0::move()
{
	float SPEED = 1;

	if (KeyboardInput::IsPressed(SCANCODE_A))
	{ 
		if (m_Scene->GetTiles()->CanScrollHoriz(-SPEED))
		{
			m_Scene->GetTiles()->Scroll(-SPEED, 0); 
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
		}
	}
	else if (KeyboardInput::IsPressed(SCANCODE_S))
	{ 
		if (m_Scene->GetTiles()->CanScrollVert(+SPEED))
			m_Scene->GetTiles()->Scroll(0, +SPEED);
	} 
}

void Layer0::onUpdate(Time ts)
{  
	curr = ts;
	move(); 

	if (cloudTimer > 0)
		cloudTimer -= ts;

	//if (cloudTimer == 0)
	//	SpawnCloud();

	Renderer::BeginScene(m_Scene); 
	Renderer::DisplaySceneTiles();
	//Renderer::DebugDisplayGrid();
	Renderer::EndScene();
}

void Layer0::onEvent(Event& e)
{    
}


//void Layer0::CloudHandler()
//{
//	Rect d1;
//	Rect d2;
//	Rect tmpBox = m_clouds.begin()->second->GetSprite()->GetBox();
//
//	Cloud* clipped = nullptr;
//
//	for (auto c : m_clouds)
//	{
//		if (!clipper.Clip(tmpBox, m_Scene->GetTiles()->GetViewWindow(), &d1, &d2))
//			clipped = c.second;
//	}
//
//	if (clipped)
//	{
//		((MovingAnimator*)clipped->GetAnimator("mov_moving"))->Stop();
//		m_clouds.erase(m_clouds.find(clipped->GetID()));
//		clipped->EntityDestroy();
//	}
//}
