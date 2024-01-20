#include "Entity.h"

//Entity::Entity(std::string type)
//{
//	m_sheet = new AnimationSheet("misc_sheet", "Assets/AnimationFilms/enemies-collectibles-sprites.bmp");
//	if (type == "elevator")
//	{
//
//	}
//}

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

void Entity::EntityDestroy()
{
	m_Sprite->Destroy();
	this->Destroy();
}

void Entity::LeftAttackPosUpdate(std::string name)
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
}

void Entity::FrameRangeFinish(Animator* animator, const Animation& anim)
{
	if (m_lookingAt == "left" && (m_state == "attacking" || m_state == "crouch_attacking"))
		m_Sprite->SetPos(startX, startY);
	else if(m_state == "moving")
		m_Sprite->SetFrame(0);
}

void Entity::FrameRangeAction(std::string name)
{	
	std::string film = name.substr(name.find_first_of('_')+1);
	m_Sprite->SetFilm(m_films[film]);
	uint32_t currFrame = ((FrameRangeAnimator*)m_animators[name])->GetCurrFrame();
	m_Sprite->SetFrame(currFrame);

	if (m_lookingAt == "left")
	{
		if ((m_state == "attacking" && currFrame == 2) ||
			(m_state == "crouch_attacking" && currFrame == 1))
			LeftAttackPosUpdate(film);
		else if (m_state == "attacking" || m_state == "crouch_attacking")
			m_Sprite->SetPos(startX, startY);
	}
}

void Entity::MovingAction(std::string name, MovingAnimator* animator)
{
	if (name == "mov_jumping") {
		m_Sprite->Move(0, -4);
	}
	else if (name.find("moving") != std::string::npos) {
		if (m_lookingAt == "left") {
			m_Sprite->Move(-2, 0);
		}
		else if (m_lookingAt == "right")
		{
			m_Sprite->Move(+2, 0);
		}
	}
	else if (name == "mov_gravity")
	{
		m_Sprite->Move(0, 1);
	}
}

void Entity::InitializeAnimators()
{
	for (auto i : m_animators) {
	

		if (i.first.find("frame_") != std::string::npos)
		{
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
		else if (i.first.find("mov_") != std::string::npos)
		{
			if (i.first == "mov_jumping") {
				i.second->SetOnStart(
					[this](Animator* animator) {
						this->m_Sprite->GetGravityHandler().SetGravityAddicted(false);
					}
				);
				
				i.second->SetOnFinish(
					[this](Animator* animator) {
						this->m_Sprite->GetGravityHandler().SetGravityAddicted(true);
						this->SetState("idle");
					}
				);

			}
			else
			{
				i.second->SetOnFinish(
					[this](Animator* animator) { this->SetState("idle"); }
				);
			}

			i.second->SetOnAction(
				[this,i](Animator* animator, const Animation& anim) { return this->MovingAction(i.first, (MovingAnimator*)i.second);  }
			);


		}
		else if (i.first.find("flash_") != std::string::npos)
		{

		}
		
	}
}

void Entity::SetState(std::string _state)
{
	m_state = _state;

	if (m_state == "idle")
	{	
		Rect rect = m_Sprite->GetBox();
		m_Sprite->GetGravityHandler().Check(rect);
	}

}

void Entity::SetLookingAt(std::string _lookingAt)
{
	m_lookingAt = _lookingAt;
}
