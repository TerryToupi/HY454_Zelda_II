#include "Guma.h"

Guma::Guma() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	SetHealth(configVars["Enemies"][2]["HP"]);
	SetDamage(configVars["Enemies"][2]["Damage"]);
	SetPoints(configVars["Enemies"][2]["Points"]);
	SetSpeed(100 - configVars["Enemies"][2]["Speed"]);

}

void Guma::throwObject() {
}
