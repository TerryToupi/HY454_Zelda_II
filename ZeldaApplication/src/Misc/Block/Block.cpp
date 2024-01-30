#include "Block.h"

Block::Block(ID _id, AnimationSheet* _sheet, Ref<Scene> _scene)
{
	m_scene = _scene;
	m_id = _id;
	m_sheet = _sheet;
	m_dead = false;
	m_lookingAt = "";

	EmplaceFilm("break_", new AnimationFilm(m_sheet, "Assets/Config/Animations/Misc/block_braking.json"));
	EmplaceAnimation(new FrameRangeAnimation("frame_breaking", 0, m_films["break_"]->GetTotalFrames(), 1, 300, 12 * 16, 100));
	EmplaceAnimator("frame_animator", new FrameRangeAnimator());

	InitializeAnimators();
}

void Block::SetRespawnCooldown(int32_t _cooldown)
{
	m_respawnCooldown = _cooldown;
}

int32_t Block::GetRespawnCooldown()
{
	return m_respawnCooldown;
}

bool Block::IsDead()
{
	return m_dead;
}

void Block::SetDead(bool _dead)
{
	m_dead = _dead;
}