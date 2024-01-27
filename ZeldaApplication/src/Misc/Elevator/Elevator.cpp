#include "Elevator.h"

Elevator::Elevator(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene)
{
	m_scene = _scene;
	m_id = _id;
	m_sheet = _sheet;
	m_state = "moving";
	m_lookingAt = "";

	EmplaceFilm("elevator_top", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/elevator_top.json"));
	EmplaceFilm("elevator_bottom", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/elevator_bottom.json"));

	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, 20));

	EmplaceAnimator("mov_moving", new MovingAnimator());
}
