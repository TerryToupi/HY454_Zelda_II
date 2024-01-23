#include "BasicPointBag.h"


BasicPointBag::BasicPointBag() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][2]["Value"]);
}
