#include "Bot.h"

Bot::Bot() {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	SetHealth(configVars["Enemies"][0]["HP"]);
	SetDamage(configVars["Enemies"][0]["Damage"]);
	SetPoints(configVars["Enemies"][0]["Points"]);
	SetSpeed(100 - configVars["Enemies"][0]["Speed"]);
}

void Bot::jump() {

}
