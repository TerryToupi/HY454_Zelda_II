#include "StrongerPointBag.h"


StrongerPointBag::StrongerPointBag() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][3]["Value"]);
}
