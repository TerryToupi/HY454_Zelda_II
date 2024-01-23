#include "ThunderSpell.h"

ThunderSpell::ThunderSpell() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setCost(configVars["Spells"][2]["Cost"]);
	setDuration(configVars["Spells"][2]["Duration"]);
	setCooldown(configVars["Spells"][2]["Cooldown"]);
}
