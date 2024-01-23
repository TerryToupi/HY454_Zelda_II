#include "LifeSpell.h"

LifeSpell::LifeSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][1]["Cost"]);
	setDuration(configVars["Spells"][1]["Duration"]);
	setCooldown(configVars["Spells"][1]["Cooldown"]);
}
