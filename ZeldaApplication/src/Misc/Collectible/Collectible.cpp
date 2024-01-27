#include "Collectinble.h"

Collectible::Collectible(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene, c_type _type)
{
	m_scene = _scene;
	m_id = _id;
	m_sheet = _sheet;
	m_type = _type;

	EmplaceFilm("key_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/key_film.json"));
}

c_type Collectible::GetType()
{
	return m_type;
}