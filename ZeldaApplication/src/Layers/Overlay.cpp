#include "Overlay.h"
using namespace Engine;

Overlay::Overlay()
	: Layer("Overlay")
{
}

void Overlay::LoadSheets()
{
	m_overlaySheets.emplace(std::make_pair("link_sheet", new AnimationSheet("link_sheet", "Assets/AnimationFilms/link-sprites.bmp")));
}

void InitilizeOverlay(Overlay* overlay)
{
	overlay->LoadSheets();
}

void Overlay::onStart()
{
	//m_Scene = MakeReference<Scene>(0);
	//m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	InitilizeOverlay(this);
	link = new Link(m_overlaySheets["link_sheet"], m_Scene);
	//link->SetSprite(m_Scene->CreateSprite("Link2", 20 * 16, 10 * 16, link->GetFilm("damage_from_right"), ""));
}

void Overlay::onUpdate(Time ts)
{

	//Renderer::BeginScene(m_Scene);
	//Renderer::DisplaySceneTiles();
	////Renderer::DebugDisplayGrid();
	//Renderer::EndScene();
}

void Overlay::onDelete()
{

}

void Overlay::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
}