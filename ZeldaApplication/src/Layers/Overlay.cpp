#include "Overlay.h"

Overlay::Overlay() : Layer("Overlay") {}

void Overlay::onStart()
{
	m_Scene = MakeReference<Scene>(0);
	m_sheet = new AnimationSheet("pause_sheet", "Assets/AnimationFilms/PAUSE.bmp");
	m_film = new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/pause_film.json");

	Application::SetOnPauseFunction([this](void) {
		ENGINE_TRACE("PAUSE");
		this->m_Scene->CreateSprite("Pause", 0, 0, this->m_film, "");
		this->onUpdate(SystemClock::GetDeltaTimeStep());
		});

	Application::SetOnResumeFunction([this](void) {
		ENGINE_TRACE("RESUME");
		this->m_Scene->RemoveSprite(this->m_Scene->GetSprite("Pause"));
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