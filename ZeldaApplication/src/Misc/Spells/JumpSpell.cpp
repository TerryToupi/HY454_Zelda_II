#include "JumpSpell.h"

JumpSpell::JumpSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][3]["Cost"]);
	setDuration(configVars["Spells"][3]["Duration"]);
	setCooldown(configVars["Spells"][3]["Cooldown"]);
	setDurationRemainingTime(0);
	setCooldownRemainingTime(0);
	setType("jumpspell");

	SetLookingAt("");
	SetState("jumpspell");

	m_sheet = new AnimationSheet("spell", "Assets/AnimationFilms/spell_visuals.bmp");
	EmplaceFilm("jumpspell_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Spells/jumpspell.json"));

	EmplaceAnimation(new FrameRangeAnimation("jumpspell_animation", 0, m_films["jumpspell_"]->GetTotalFrames(), 1, 300, 12 * 36, 70));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();
}
