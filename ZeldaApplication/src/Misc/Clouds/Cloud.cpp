#include "Cloud.h"

Cloud::Cloud(uint32_t _id, AnimationSheet* _sheet, Ref<Scene> _scene, uint32_t cloudNum, uint32_t speed)
{
	m_id = _id;
	m_sheet = _sheet;
	m_scene = _scene;
	m_lookingAt = "left";
	
	EmplaceFilm("cloud_film", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/cloud_" + std::to_string(cloudNum) + ".json"));
	EmplaceAnimation(new MovingAnimation("mov_moving", 0, 0, 0, speed));
	EmplaceAnimator("mov_moving", new MovingAnimator());
	
	InitializeAnimators();
}