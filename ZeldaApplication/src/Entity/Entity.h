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
	void LeftAttackPosUpdate(std::string name);

	void SetState(std::string _state);
	void SetLookingAt(std::string _loookingAt);
	std::string GetState();
	std::string GetLookingAt();

public:
	void FrameRangeFinish(Animator* animator, const Animation& anim);
	void FrameRangeAction(std::string name);
	void FrameRangeStart(std::string name);
	void InitializeAnimators();

protected:

	Sprite m_Sprite;
	AnimationSheet* m_sheet;
	Films m_films;
	Animations m_animations;
	Animators m_animators;

	std::string m_state;
	std::string m_lookingAt;

private:
	uint32_t startX;
	uint32_t startY;
};
