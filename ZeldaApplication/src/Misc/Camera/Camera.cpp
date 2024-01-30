#include "Camera.h"

Camera::Camera(Ref<Scene> _scene)
{
	m_scene = _scene;
	m_moving = new MovingAnimator();
	m_anim = new MovingAnimation("mov_moving", 0, 0, 0, 20);
	InitializeAnimator();
}

MovingAnimation* Camera::GetAnimation()
{
	return m_anim;
}

MovingAnimator* Camera::GetAnimator()
{
	return m_moving;
}

void Camera::InitializeAnimator()
{
	m_moving->SetOnAction(
		[this](Animator* animator, const Animation& anim) { 
			if (this->GetDirection() == "right" && m_scene->GetTiles()->CanScrollHoriz(+2))
				m_scene->GetTiles()->Scroll(+2, 0);
			else if (this->GetDirection() == "left" && m_scene->GetTiles()->CanScrollHoriz(-2))
				m_scene->GetTiles()->Scroll(-2, 0);
		}
	);
}

std::string Camera::GetDirection()
{
	return m_direction;
}

void Camera::SetDirection(std::string _direction)
{
	m_direction = _direction;
}