#include "Layer2.h"
using namespace Engine;



Layer2::Layer2()
	: Layer("Layer2")
{
}

void Layer2::LoadSheets()
{
	m_sheet = new AnimationSheet("link_sheet", "Assets/AnimationFilms/link_sprites.bmp");

}

void InitilizeLayer(Layer2* overlay)
{

}

void Layer2::onStart()
{
	m_Scene = MakeReference<Scene>(2);
//	InitilizeLayer(this);
//	LoadSheets();
	m_sheet = new AnimationSheet("info_sheet", "Assets/AnimationFilms/overlay.bmp");
	//m_film = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/life_100.json");
	for (int i = 0; i < 10; i++) {
		m_numberfilm[i] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Numbers/"+ std::to_string(i) +".json");
	}
	for (int i = 0; i < 5; i++) {
		m_magicpointfilm[i] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/magic_"+ std::to_string((i+1) * 20) + ".json");
	}

	for (int i = 0; i < 5; i++) {
		m_healthfilm[i] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/life_" + std::to_string((i + 1) * 20) + ".json");
	}

	m_lifespellFilm[0] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/lifespellActive.json");
	m_lifespellFilm[1] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/lifespellNotActive.json");
	m_jumpspellFilm[0] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/jumpspellActive.json");
	m_jumpspellFilm[1] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/jumpspellNotActive.json");
	m_shieldspellFilm[0] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/shieldspellActive.json");
	m_shieldspellFilm[1] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/shieldspellNotActive.json");
	m_thunderspellFilm[0] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/thunderspellActive.json");
	m_thunderspellFilm[1] = new AnimationFilm(m_sheet, "Assets/Config/Animations/Info/thunderspellNotActive.json");


	currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[4], "");
	//currHealthNumSprite[0] = m_Scene->CreateSprite("healthHundrends", 2 * 16 + 8, 8, m_numberfilm[1], "");
	//currHealthNumSprite[1] = m_Scene->CreateSprite("healthTens", 3 * 16, 8, m_numberfilm[0], "");
	//currHealthNumSprite[2] = m_Scene->CreateSprite("healthOnes", 3 * 16 + 8, 8, m_numberfilm[0], "");

	currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[4], "");
	//currMagicNumSprite[0] = m_Scene->CreateSprite("hundrends", 7 * 16 + 12, 8, m_numberfilm[1], "");
	//currMagicNumSprite[1] = m_Scene->CreateSprite("tens", 8 * 16 + 4, 8, m_numberfilm[0], "");
	//currMagicNumSprite[2] = m_Scene->CreateSprite("ones", 8 * 16 + 12, 8, m_numberfilm[0], "");

	currLifeIconSprite = m_Scene->CreateSprite("lifeIcon", 13 * 16, 1, m_lifespellFilm[0], "");
	currJumpIconSprite = m_Scene->CreateSprite("jumpIcon", 15 * 16, 1, m_jumpspellFilm[0], "");
	currShieldIconSprite = m_Scene->CreateSprite("shieldIcon", 17 * 16, 1, m_shieldspellFilm[0], "");
	currThunderIconSprite = m_Scene->CreateSprite("thunderIcon", 19 * 16, 1, m_thunderspellFilm[0], "");

	currHealthNum = 0;
	currMagicNum = 0;
	currPointNum = layer1.link->getPoints();
}

void Layer2::onUpdate(Time ts)
{

	if (currHealthNum != layer1.link->getHealth() && layer1.link->getHealth() > 0) {
		currHealthNum = layer1.link->getHealth();
		m_Scene->RemoveSprite(currHealthSprite);

		if (currHealthNum > (layer1.link->getMaxHealth()/5) * 4 ) {
			currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[4], "");
		}else if (currHealthNum > (layer1.link->getMaxHealth() / 5) * 3) {
			currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[3], "");
		}else if (currHealthNum > (layer1.link->getMaxHealth() / 5) * 2) {
			currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[2], "");
		}else if(currHealthNum > (layer1.link->getMaxHealth() / 5) * 1){
			currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[1], "");
		}else if (currHealthNum > 0) {
			currHealthSprite = m_Scene->CreateSprite("healthBoxes", 1 * 16, 8, m_healthfilm[0], "");
		}


		//m_Scene->RemoveSprite(currHealthNumSprite[0]);
		//m_Scene->RemoveSprite(currHealthNumSprite[1]);
		//m_Scene->RemoveSprite(currHealthNumSprite[2]);
		//currHealthNumSprite[0] = m_Scene->CreateSprite("healthHundrends", 2 * 16 + 8, 8, m_numberfilm[currHealthNum / 100], "");
		//currHealthNumSprite[1] = m_Scene->CreateSprite("healthTens", 3 * 16, 8, m_numberfilm[(currHealthNum %100 ) / 10], "");
		//currHealthNumSprite[2] = m_Scene->CreateSprite("healthOnes", 3 * 16 + 8, 8, m_numberfilm[(currHealthNum % 10)], "");
	}

	if (currMagicNum != layer1.link->getMagicPoints() && layer1.link->getMagicPoints() > 0) {
		currMagicNum = layer1.link->getMagicPoints();
		m_Scene->RemoveSprite(currMagicSprite);

		if (currMagicNum > (layer1.link->getMaxMagicPoints() / 5) * 4) {
			currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[4], "");
		}
		else if (currMagicNum > (layer1.link->getMaxMagicPoints() / 5) * 3) {
			currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[3], "");
		}
		else if (currMagicNum > (layer1.link->getMaxMagicPoints() / 5) * 2) {
			currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[2], "");
		}
		else if (currMagicNum > (layer1.link->getMaxMagicPoints() / 5) * 1) {
			currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[1], "");
		}
		else if (currMagicNum > 0) {
			currMagicSprite = m_Scene->CreateSprite("magicBoxes", 5 * 16, 8, m_magicpointfilm[0], "");
		}

		//m_Scene->RemoveSprite(currMagicNumSprite[0]);
		//m_Scene->RemoveSprite(currMagicNumSprite[1]);
		//m_Scene->RemoveSprite(currMagicNumSprite[2]);
		//currMagicNumSprite[0] = m_Scene->CreateSprite("hundrends", 7 * 16 + 12, 8, m_numberfilm[currMagicNum / 100], "");
		//currMagicNumSprite[1] = m_Scene->CreateSprite("tens", 8 * 16 + 4, 8, m_numberfilm[(currMagicNum % 100) / 10], "");
		//currMagicNumSprite[2] = m_Scene->CreateSprite("ones", 8 * 16 + 12, 8, m_numberfilm[(currMagicNum % 10)], "");
	}

	currLifeIconSprite = UpdateCooldowns("life", layer1.link->lifespell, lifespellDisabled, currLifeCooldown, currLifeIconSprite, m_lifespellFilm, 13);
	currJumpIconSprite = UpdateCooldowns("jump", layer1.link->jumpspell , jumpspellDisabled, currJumpCooldown, currJumpIconSprite, m_jumpspellFilm, 15);
	currShieldIconSprite = UpdateCooldowns("shield", layer1.link->shieldspell, shieldspellDisabled, currShieldCooldown, currShieldIconSprite, m_shieldspellFilm, 17);
	currThunderIconSprite = UpdateCooldowns("thunder", layer1.link->thunderspell, thunderspellDisabled, currThunderCooldown, currThunderIconSprite, m_thunderspellFilm, 19);

	Renderer::BeginScene(m_Scene);
	Renderer::DisplaySprites();
	Renderer::EndScene();
}

Sprite Layer2::UpdateCooldowns(std::string type, Spell spell, bool &isDisabled, Sprite cooldownSprite[], Sprite iconSprite, AnimationFilm* film[], int posX) {
	if (!spell.canUse() && !spell.isActive() && isDisabled == false) {
		m_Scene->RemoveSprite(iconSprite);
		iconSprite = m_Scene->CreateSprite(type + "Icon", posX * 16, 1, film[1], "");
		isDisabled = true;
	}
	else if (spell.canUse() && isDisabled == true) {
		m_Scene->RemoveSprite(iconSprite);
		iconSprite = m_Scene->CreateSprite(type + "Icon", posX * 16, 1, film[0], "");
		isDisabled = false;

		if (m_Scene->HasSprite(type + "Tens"))
			m_Scene->RemoveSprite(cooldownSprite[0]);
		if (m_Scene->HasSprite(type + "Ones"))
			m_Scene->RemoveSprite(cooldownSprite[1]);
	}
	if (isDisabled == true) {
		if(m_Scene->HasSprite(type + "Tens"))
			m_Scene->RemoveSprite(cooldownSprite[0]);
		if (m_Scene->HasSprite(type + "Ones"))
			m_Scene->RemoveSprite(cooldownSprite[1]);
		
		cooldownSprite[0] = m_Scene->CreateSprite(type + "Tens", posX * 16 + 2, 22, m_numberfilm[spell.getCooldownRemainingTime() / 10000], "");
		cooldownSprite[1] = m_Scene->CreateSprite(type + "Ones", posX * 16 + 10, 22, m_numberfilm[(spell.getCooldownRemainingTime() / 1000) % 10], "");
	}
	else if (spell.isActive()) {
		if (m_Scene->HasSprite(type + "Tens"))
			m_Scene->RemoveSprite(cooldownSprite[0]);
		if (m_Scene->HasSprite(type + "Ones"))
			m_Scene->RemoveSprite(cooldownSprite[1]);

		cooldownSprite[0] = m_Scene->CreateSprite(type + "Tens", posX * 16 + 2, 22, m_numberfilm[spell.getDurationRemainingTime() / 10000], "");
		cooldownSprite[1] = m_Scene->CreateSprite(type + "Ones", posX * 16 + 10, 22, m_numberfilm[(spell.getDurationRemainingTime() / 1000) % 10], "");
	}

	return iconSprite;
}

void Layer2::onDelete() {}

void Layer2::onEvent(Event& e){}