#include "Staflos.h"

Staflos::Staflos(){
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);
	
	SetHealth(configVars["Enemies"][3]["HP"]);
	SetDamage(configVars["Enemies"][3]["Damage"]);
	SetPoints(configVars["Enemies"][3]["Points"]);
	SetSpeed(100 - configVars["Enemies"][3]["Speed"]);
}

void Staflos::apearFromCeiling() {
}
