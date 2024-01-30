#pragma once

#include <Engine.h>

using namespace Engine;

class Camera{
public:
	Camera(Ref<Scene> _scene);
	~Camera() = default;

	void SetDirection(std::string _direction);
	std::string GetDirection();

	void InitializeAnimator();
	MovingAnimator* GetAnimator();
	MovingAnimation* GetAnimation();


private:
	MovingAnimator* m_moving;
	MovingAnimation* m_anim;
	std::string m_direction;

	Ref<Scene> m_scene;
};
