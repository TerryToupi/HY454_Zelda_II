#include "Layer2.h"
using namespace Engine;

Layer2::Layer2()
	: Layer("Layer2")
{
}

void Layer2::LoadSheets()
{
	m_overlaySheets.emplace(std::make_pair("link_sheet", new AnimationSheet("link_sheet", "Assets/AnimationFilms/link-sprites.bmp")));
}

void InitilizeLayer2(Layer2* overlay)
{
	overlay->LoadSheets();
}

void Layer2::onStart()
{
	//m_Scene = MakeReference<Scene>(0);
	//m_Scene->GetTiles()->LoadTiles("Assets/TileSet/Zelda-II-Parapa-Palace-Tileset.bmp");
	InitilizeLayer2(this);
	link = new Link(m_overlaySheets["link_sheet"], m_Scene);
	//link->SetSprite(m_Scene->CreateSprite("Link2", 20 * 16, 10 * 16, link->GetFilm("damage_from_right"), ""));
}

void Layer2::onUpdate(Time ts)
{

	//Renderer::BeginScene(m_Scene);
	//Renderer::DisplaySceneTiles();
	////Renderer::DebugDisplayGrid();
	//Renderer::EndScene();
}

void Layer2::onDelete()
{

}

void Layer2::onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
}