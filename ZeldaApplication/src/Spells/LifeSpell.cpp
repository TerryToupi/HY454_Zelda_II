#include "LifeSpell.h"

LifeSpell::LifeSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][1]["Cost"]);
	setDuration(configVars["Spells"][1]["Duration"]);
	setCooldown(configVars["Spells"][1]["Cooldown"]);
	setDurationRemainingTime(0);
	setCooldownRemainingTime(0);
	setType("lifespell");

	SetLookingAt("");
	SetState("lifespell");

	m_sheet = new AnimationSheet("spell", "Assets/AnimationFilms/spell_visuals.bmp");
	EmplaceFilm("lifespell_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Spells/healspell.json"));

	EmplaceAnimation(new FrameRangeAnimation("lifespell_animation", 0, m_films["lifespell_"]->GetTotalFrames(), 0, 300, 12*36, 150));

	EmplaceAnimator("lifespell_animator", new FrameRangeAnimator());

	InitializeAnimators();
}
