#include "ExtraLife.h"


ExtraLife::ExtraLife() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][5]["Value"]);
}
