#include "ShieldSpell.h"

ShieldSpell::ShieldSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][0]["Cost"]);
	setDuration(configVars["Spells"][0]["Duration"]);
	setCooldown(configVars["Spells"][0]["Cooldown"]);
	setDurationRemainingTime(0);
	setCooldownRemainingTime(0);
	setType("shieldspell");

	SetLookingAt("");
	SetState("shieldspell");

	m_sheet = new AnimationSheet("spell", "Assets/AnimationFilms/spell_visuals.bmp");
	EmplaceFilm("shieldspell_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Spells/shieldspell.json"));

	EmplaceAnimation(new FrameRangeAnimation("shieldspell_animation", 0, m_films["shieldspell_"]->GetTotalFrames(), 0, 300, 12 * 36, 30));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();
}
