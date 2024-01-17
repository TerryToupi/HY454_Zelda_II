#pragma once

#include <Engine.h>

using namespace Engine;

class Entity {
public:
	using Films = std::unordered_map<std::string, AnimationFilm*>;
	using Animations = std::unordered_map<std::string, Animation*>;
	using Animators = std::unordered_map<std::string, Animator*>;

	Entity() = default;
	~Entity() = default;

	Animation* GetAnimation(std::string name);
	AnimationFilm* GetFilm(std::string name);
	Animator* GetAnimator(std::string name);
	Sprite GetSprite();

	void EmplaceAnimation(Animation* animation);
	void EmplaceFilm(std::string id, AnimationFilm* film);
	void EmplaceAnimator(std::string id, Animator* animator);
	void SetSprite(Sprite s);

	void FrameRangerFinish(Animator* animator, const Animation& anim);

protected:

	Sprite m_Sprite;
	AnimationSheet* m_sheet;
	Films m_films;
	Animations m_animations;
	Animators m_animators;
};
