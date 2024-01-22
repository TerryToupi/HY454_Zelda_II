#include "Wosu.h"

Wosu::Wosu(uint32_t _id, std::string _direction) {
	setHealth(8);
	setDamage(10);
	setPoints(0);
	m_id = _id;
	m_lookingAt = _direction;
	m_sheet = new AnimationSheet("guma_sheet", "Assets/AnimationFilms/enemies-collectibles-sprites.bmp");
	EmplaceFilm("moving_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Wosu/moving_left.json"));
	EmplaceFilm("moving_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Wosu/moving_right.json"));

	EmplaceAnimation(new FrameRangeAnimation("frame_moving_left", 0, m_films["moving_left"]->GetTotalFrames(), 0, 300, 12 * 16, 150));
	EmplaceAnimation(new FrameRangeAnimation("frame_moving_right", 0, m_films["moving_right"]->GetTotalFrames(), 0, 300, 12 * 16, 150));
	EmplaceAnimation(new MovingAnimation("mov_moving_left", 0, 0, 0, 20));
	EmplaceAnimation(new MovingAnimation("mov_moving_right", 0, 0, 0, 20));

	EmplaceAnimator("frame_moving_left", new FrameRangeAnimator());
	EmplaceAnimator("frame_moving_right", new FrameRangeAnimator());
	EmplaceAnimator("mov_moving_left", new MovingAnimator());
	EmplaceAnimator("mov_moving_right", new MovingAnimator());

	InitializeAnimators();
}

