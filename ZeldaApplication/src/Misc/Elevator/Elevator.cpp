#include "Elevator.h"

Elevator::Elevator(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene)
{
	m_scene = _scene;
	m_id = _id;
	m_sheet = _sheet;
	m_state = "idle";
	m_lookingAt = "";

	EmplaceFilm("elevator_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/elevator.json"));

	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, 20));

	EmplaceAnimator("mov_moving", new MovingAnimator());

	InitializeAnimators();
}
