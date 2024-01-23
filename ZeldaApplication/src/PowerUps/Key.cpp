#include "Key.h"


Key::Key() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	setValue(configVars["Power-ups"][4]["Value"]);
}
