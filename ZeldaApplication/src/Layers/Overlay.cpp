#include "Overlay.h"

Overlay::Overlay() : Layer("Overlay") {}

void Overlay::onStart()
{
	m_Scene = MakeReference<Scene>(0);
	m_sheetPause = new AnimationSheet("pause_sheet", "Assets/AnimationFilms/PAUSE.bmp");
	m_filmPause = new AnimationFilm(m_sheetPause, "Assets/Config/Animations/Misc/pause_film.json");
	m_sheetOver = new AnimationSheet("over_sheet", "Assets/AnimationFilms/GAME_OVER.bmp");
	m_filmOver = new AnimationFilm(m_sheetOver, "Assets/Config/Animations/Misc/over_film.json");
	m_sheetWin = new AnimationSheet("over_sheet", "Assets/AnimationFilms/WIN.bmp");
	m_filmWin = new AnimationFilm(m_sheetWin, "Assets/Config/Animations/Misc/win_film.json");

	Application::SetOnPauseFunction([this](void) {
		ENGINE_TRACE("PAUSE");
		this->m_Scene->CreateSprite("Pause", 0, 0, this->m_filmPause, "");
		this->onUpdate(SystemClock::GetDeltaTimeStep());
		});

	Application::SetOnResumeFunction([this](void) {
		ENGINE_TRACE("RESUME");
		this->m_Scene->RemoveSprite(this->m_Scene->GetSprite("Pause"));
		});

	Application::SetOnFreezeFunction([this](void) {
		ENGINE_TRACE("FROZEN");
		if (this->layer1.link->getLives() == 0)
		{
			this->m_Scene->CreateSprite("Over", 0, 0, this->m_filmOver, "");
		}
		else
		{
			this->m_Scene->CreateSprite("Win", 0, 0, this->m_filmWin, "");
		}

		this->onUpdate(SystemClock::GetDeltaTimeStep());
		});

}

void Overlay::onUpdate(Time ts) 
{
	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySprites();
	Renderer::EndScene();
}

void Overlay::onEvent(Event& e) {};
void Overlay::onDelete() {};