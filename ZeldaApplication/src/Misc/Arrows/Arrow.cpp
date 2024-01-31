#include "Arrow.h"

Arrow::Arrow(uint32_t _id, AnimationSheet* _sheet, Ref<Scene> _scene)
{
	m_id = _id;
	m_sheet = _sheet;
	m_scene = _scene;

	EmplaceFilm("arrow_left", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/arrow_left.json"));
	EmplaceFilm("arrow_right", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/arrow_right.json"));

	EmplaceAnimation(new FrameRangeAnimation("frame_arrow_left", 0, m_films["arrow_left"]->GetTotalFrames(), 2, 300, 12 * 16, 100));
	EmplaceAnimation(new FrameRangeAnimation("frame_arrow_right", 0, m_films["arrow_right"]->GetTotalFrames(), 2, 300, 12 * 16, 100));
	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, 20));
	EmplaceAnimation(new MovingAnimation("mov_gravity", 2, 0, 0, 6));

	EmplaceAnimator("mov_moving", new MovingAnimator());
	EmplaceAnimator("frame_animator", new FrameRangeAnimator());
	EmplaceAnimator("mov_gravity", new MovingAnimator());

	InitializeAnimators();
}
