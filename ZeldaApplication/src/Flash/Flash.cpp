#include "Flash.h"

Flash::Flash() {
	SetLookingAt("");
	SetState("flash");

	m_sheet = new AnimationSheet("flash", "Assets/AnimationFilms/flash.bmp");
	
	EmplaceFilm("flash_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Flash/flash_animation.json"));

	EmplaceAnimation(new FrameRangeAnimation("flash_animation", 0, m_films["flash_"]->GetTotalFrames(), 1, 0, 0, 40));

	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();
}
