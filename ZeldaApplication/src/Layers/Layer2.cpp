#include "Layer2.h"
using namespace Engine;

Layer2::Layer2()
	: Layer("Layer2")
{
}

void Layer2::LoadSheets()
{
	m_sheet = new AnimationSheet("link_sheet", "Assets/AnimationFilms/link_sprites.bmp");

}

void InitilizeLayer(Layer2* overlay)
{
	overlay->LoadSheets();
	overlay->m_film = new AnimationFilm(overlay->m_sheet, "Assets/Config/Animations/Link/moving_right.json");
}

void Layer2::onStart()
{
	m_Scene = MakeReference<Scene>(2);
//	InitilizeLayer(this);
//	LoadSheets();
	m_sheet = new AnimationSheet("info_sheet", "Assets/AnimationFilms/overlay.bmp");
	m_film = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/life_100.json");
	m_Scene->CreateSprite("lifebar", 9 * 16, 8, m_film, "");
}

void Layer2::onUpdate(Time ts)
{
	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySprites();
	Renderer::EndScene();
}

void Layer2::onDelete() {}

void Layer2::onEvent(Event& e){}