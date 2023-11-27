#include "skoupidi.h"
#include "EngineApp.h"

using namespace Engine;

skoupidi::skoupidi()
	: Layer("skoupidi")
{
}

void skoupidi::onAttach()
{
	m_Scene = new Scene();
	Engine::Entity makis = m_Scene->m_Graph->createEntity("makis");
	Engine::Entity takis = m_Scene->m_Graph->createEntity("takis");
	Engine::Entity sakis = m_Scene->m_Graph->createEntity("sakis");
	Engine::Entity akis = m_Scene->m_Graph->createEntity("akis");
	Engine::Entity dimitroglou = m_Scene->m_Graph->createEntity("dimitroglou");
	m_Scene->m_Graph->addEntityToRoot(makis);
	m_Scene->m_Graph->addEntityToRoot(takis);
	m_Scene->m_Graph->addEntityToRoot(sakis);
	m_Scene->m_Graph->addEntityToRoot(akis);
	m_Scene->m_Graph->addEntityToRoot(dimitroglou);
}

void skoupidi::onDettach()
{
}

void skoupidi::onUpdate()
{
}