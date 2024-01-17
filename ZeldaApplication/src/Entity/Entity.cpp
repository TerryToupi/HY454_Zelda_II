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

void Entity::FrameRangeFinish(Animator* animator, const Animation& anim)
{
	FrameRangeAnimator* a = (FrameRangeAnimator*)animator;
	FrameRangeAnimation* film = (FrameRangeAnimation*)anim.Clone();
}

void Entity::FrameRangeAction(std::string name)
{
	m_Sprite->SetFilm(m_films[name]);
	m_Sprite->SetFrame(((FrameRangeAnimator*)m_animators[name])->GetCurrFrame());
}

void Entity::InitializeAnimators()
{
	for (auto i : m_animators) {
		i.second->SetOnAction(
			[this,i](Animator* animator, const Animation& anim) { return this->FrameRangeAction(i.first); }
		);

		i.second->SetOnFinish(
			[this,i](Animator* animator) { return this->FrameRangeFinish(animator, *(this->GetAnimation(i.first))); }
		);

	}
}
