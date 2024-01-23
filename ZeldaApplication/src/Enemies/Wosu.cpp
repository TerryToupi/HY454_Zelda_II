#include "Wosu.h"

Wosu::Wosu(uint32_t _id, std::string _direction, uint32_t _stage) {
	SetHealth(8);
	SetDamage(10);
	SetPoints(0);
	m_stage = _stage;
	m_state = "amblambloubla";
	m_id = _id;
	m_lookingAt = _direction;
	m_sheet = new AnimationSheet("guma_sheet", "Assets/AnimationFilms/enemies-collectibles-sprites.bmp");
	EmplaceFilm("moving_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Wosu/moving_left.json"));
	EmplaceFilm("moving_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Wosu/moving_right.json"));
	EmplaceFilm("death_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/double_damage.json"));

	EmplaceAnimation(new FrameRangeAnimation("frame_moving_left", 0, m_films["moving_left"]->GetTotalFrames(), 0, 300, 12 * 16, 150));
	EmplaceAnimation(new FrameRangeAnimation("frame_moving_right", 0, m_films["moving_right"]->GetTotalFrames(), 0, 300, 12 * 16, 150));
	EmplaceAnimation(new FrameRangeAnimation("frame_death", 0, m_films["death_"]->GetTotalFrames(), 1, 300, 12 * 16, 50));
	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, 50));
	EmplaceAnimation(new MovingAnimation("mov_gravity", 0, 0, 0, 4));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());
	EmplaceAnimator("mov_moving", new MovingAnimator());
	EmplaceAnimator("mov_gravity", new MovingAnimator());

	InitializeAnimators();
}

