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
}
