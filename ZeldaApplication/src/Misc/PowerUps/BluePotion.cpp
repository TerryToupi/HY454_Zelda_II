#include "BluePotion.h"


BluePotion::BluePotion() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][0]["Value"]);
}
