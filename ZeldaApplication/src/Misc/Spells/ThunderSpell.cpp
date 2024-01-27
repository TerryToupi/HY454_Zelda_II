#include "ThunderSpell.h"

ThunderSpell::ThunderSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][2]["Cost"]);
	setDuration(configVars["Spells"][2]["Duration"]);
	setCooldown(configVars["Spells"][2]["Cooldown"]);
	setDurationRemainingTime(0);
	setCooldownRemainingTime(0);
	setType("thunderspell");

	SetLookingAt("");
	SetState("thunderspell");

	m_sheet = new AnimationSheet("spell", "Assets/AnimationFilms/spell_visuals.bmp");
	EmplaceFilm("thunderspell_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Spells/thunderspell.json"));

	EmplaceAnimation(new FrameRangeAnimation("thunderspell_animation", 0, m_films["thunderspell_"]->GetTotalFrames(), 1, 300, 12 * 36, 50));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();

}
