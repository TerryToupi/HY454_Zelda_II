#pragma once 

#include <Engine.h>
#include "../Link/Link.h"
#include "layer1.h"
#include <json.hpp>



using json = nlohmann::json;
using namespace Engine;

class Layer2 : public Layer
{
public:
    Layer2();

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override;

	void onEvent(Event& e) override;

	void LoadSheets();
	
	Sprite UpdateCooldowns(std::string type, Spell spell, bool &isDisabled, Sprite sprite[], Sprite iconSprite, AnimationFilm* film[], int posX);

	AnimationSheet* m_sheet;
	AnimationSheet* m_collectiblesSheet;

	AnimationFilm* m_numberfilm[10];
	AnimationFilm* m_healthfilm[5];
	AnimationFilm* m_magicpointfilm[5];
	AnimationFilm* m_lifespellFilm[2];
	AnimationFilm* m_jumpspellFilm[2];
	AnimationFilm* m_shieldspellFilm[2];
	AnimationFilm* m_thunderspellFilm[2];
	AnimationFilm* m_keyFilm;

	Sprite currHealthSprite;
	Sprite currMagicSprite;
	Sprite currLifeIconSprite;
	Sprite currJumpIconSprite;
	Sprite currShieldIconSprite;
	Sprite currThunderIconSprite;
	Sprite totalLivesSprite;
	Sprite keySprite;
	Sprite totalKeysSprite;

	Sprite currLifeCooldown[2];
	Sprite currJumpCooldown[2];
	Sprite currShieldCooldown[2];
	Sprite currThunderCooldown[2];

	int currHealthNum;
	int currMagicNum;
	int currPointNum;
	int currLives;
	int currKeys;
	bool lifespellDisabled = false;
	bool jumpspellDisabled = false;
	bool shieldspellDisabled = false;
	bool thunderspellDisabled = false;

	Layer1& layer1 = (Layer1&)(Application::GetLayer("Layer1"));
};