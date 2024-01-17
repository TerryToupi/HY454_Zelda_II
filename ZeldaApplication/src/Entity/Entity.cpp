#include "Entity.h"

Animation* Entity::GetAnimation(std::string name)
{
	return m_animations.at(name);
}

AnimationFilm* Entity::GetFilm(std::string name)
{
	return m_films.at(name);
}

Animator* Entity::GetAnimator(std::string name)
{
	return m_animators.at(name);
}

Sprite Entity::GetSprite()
{
	return m_Sprite;
}

std::string Entity::GetState()
{
	return m_state;
}

std::string Entity::GetLookingAt()
{
	return m_lookingAt;
}

void Entity::EmplaceAnimation(Animation* animation) 
{
	m_animations.emplace(std::make_pair(animation->GetId(), animation));
}

void Entity::EmplaceFilm(std::string id, AnimationFilm* film)
{
	m_films.emplace(std::make_pair(id, film));
}

void Entity::EmplaceAnimator(std::string id, Animator* animator)
{
	m_animators.emplace(std::make_pair(id, animator));
}

void Entity::SetSprite(Sprite s)
{
	m_Sprite = s;
}

void Entity::Reposition(std::string name)
{
	uint32_t offset = 0;
	uint32_t w = 0;
	uint32_t currX = m_Sprite->GetPosX();
	uint32_t currY = m_Sprite->GetPosY();


	w = m_films[name]->GetFrameBox(2).w;
	offset = w / 2;
	if (currX > offset) {
		m_Sprite->SetPos(currX - offset, currY);
	}
	
}

void Entity::FrameRangeStart(std::string name) 
{
	startX = m_Sprite->GetPosX();
	startY = m_Sprite->GetPosY();
	m_Sprite->SetFilm(m_films[name]);
}

void Entity::FrameRangeFinish(Animator* animator, const Animation& anim)
{
	if (m_lookingAt == "left" && (m_state == "attacking" || m_state == "crouch_attacking"))
		m_Sprite->SetPos(startX, startY);
}

void Entity::FrameRangeAction(std::string name)
{

	uint32_t currFrame = ((FrameRangeAnimator*)m_animators[name])->GetCurrFrame();
	m_Sprite->SetFrame(currFrame);

	ENGINE_CORE_TRACE("{0}", currFrame);

	if (m_lookingAt == "left")
	{
		if ((m_state == "attacking" && currFrame == 2) || 
			(m_state == "crouch_attacking" && currFrame == 1))
			Reposition(name);
	}
}

void Entity::InitializeAnimators()
{
	for (auto i : m_animators) {
		i.second->SetOnStart(
			[this,i](Animator* animator) { return this->FrameRangeStart(i.first); }
		);

		i.second->SetOnAction(
			[this,i](Animator* animator, const Animation& anim) { return this->FrameRangeAction(i.first); }
		);

		i.second->SetOnFinish(
			[this,i](Animator* animator) { return this->FrameRangeFinish(animator, *(this->GetAnimation(i.first))); }
		);

	}
}
