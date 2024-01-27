#include "Collectinble.h"

Collectible::Collectible(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene, c_type _type)
{
	m_scene = _scene;
	m_id = _id;
	m_sheet = _sheet;
	m_type = _type;

	EmplaceFilm("key_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/key_film.json"));
	EmplaceFilm("bluePotion_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/blue_potion_film.json"));
	EmplaceFilm("redPotion_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/red_potion_film.json"));
	EmplaceFilm("extraLife_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/extra_life_film.json"));
	EmplaceFilm("simplePointBag_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/point_bag_film.json"));
	EmplaceFilm("strongerPointBag_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/point_bag_film.json"));
}

c_type Collectible::GetType()
{
	return m_type;
}

int Collectible::getCooldown() const {
	return cooldown;
}

void Collectible::setCooldown(int newCooldown) {
	cooldown = newCooldown;
}