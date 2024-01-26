#include "Door.h"

Door::Door(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene)
{
	m_state = "closed";
	m_id = _id;
	m_lookingAt = "";
	m_sheet = _sheet;
	m_scene = _scene;
	EmplaceFilm("open_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/door_opening.json"));
	EmplaceAnimation(new FrameRangeAnimation("frame_open", 0, m_films["open_"]->GetTotalFrames(), 1, 300, 12 * 16, 30));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();
}