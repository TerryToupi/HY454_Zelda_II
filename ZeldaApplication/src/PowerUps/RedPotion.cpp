#include "RedPotion.h"


RedPotion::RedPotion() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][1]["Value"]);
}
