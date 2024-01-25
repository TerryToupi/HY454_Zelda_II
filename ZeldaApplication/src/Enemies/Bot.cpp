#include "Bot.h"

Bot::Bot(uint32_t _id, std::string _direction, uint32_t _stage, AnimationSheet* _sheet) {
	std::ifstream file("Assets/Config/Variables/ConfigVariables.json");
	json configVars = json::parse(file);

	SetHealth(configVars["Enemies"][0]["HP"]);
	SetDamage(configVars["Enemies"][0]["Damage"]);
	SetPoints(configVars["Enemies"][0]["Points"]);
	SetSpeed(100 - configVars["Enemies"][0]["Speed"]);

	m_stage = _stage;
	m_state = "amblambloubla";
	m_id = _id;
	m_lookingAt = _direction;
	m_sheet = _sheet;
	EmplaceFilm("moving_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Great Palace Bot/slime_moving.json"));
	EmplaceFilm("moving_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Great Palace Bot/slime_moving.json"));
	EmplaceFilm("death_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/single_damage.json"));

	EmplaceAnimation(new FrameRangeAnimation("frame_moving_left", 0, m_films["moving_left"]->GetTotalFrames(), 0, 300, 12 * 16, 150));
	EmplaceAnimation(new FrameRangeAnimation("frame_moving_right", 0, m_films["moving_right"]->GetTotalFrames(), 0, 300, 12 * 16, 150));

	EmplaceAnimation(new FrameRangeAnimation("frame_death", 0, m_films["death_"]->GetTotalFrames(), 1, 300, 12 * 16, 50));
	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, GetSpeed()));

	EmplaceAnimation(new MovingAnimation("mov_gravity", 0, 0, 0, 4));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());
	EmplaceAnimator("mov_moving", new MovingAnimator());
	EmplaceAnimator("mov_gravity", new MovingAnimator());

	InitializeAnimators();
}

void Bot::jump() {

}
