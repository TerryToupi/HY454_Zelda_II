#include "ShieldSpell.h"

ShieldSpell::ShieldSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][0]["Cost"]);
	setDuration(configVars["Spells"][0]["Duration"]);
	setCooldown(configVars["Spells"][0]["Cooldown"]);
}
