#pragma once

#include <Engine.h>
#include <json.hpp>

using json = nlohmann::json;
using namespace Engine;

class Entity : public LatelyDestroyable{
public:
	using Films = std::unordered_map<std::string, AnimationFilm*>;
	using Animations = std::unordered_map<std::string, Animation*>;
	using Animators = std::unordered_map<std::string, Animator*>;

	Entity() = default;
	Entity(std::string type);
	

	Animation* GetAnimation(std::string name);
	AnimationFilm* GetFilm();
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
	uint32_t GetID();

	void EntityDestroy();

public:
	void FrameRangeFinish();
	void FrameRangeAction(FrameRangeAnimator* animator);
	void FrameRangeStart(std::string name);

	void MovingAction(std::string name, MovingAnimator* animator);

	void InitializeAnimators();

protected:
	
	Sprite m_Sprite;
	AnimationSheet* m_sheet;
	Films m_films;
	Animations m_animations;
	Animators m_animators;

	Animation* m_singleAnim;
	AnimationFilm* m_singleFilm;

	std::string m_state;
	std::string m_lookingAt;
	std::string m_type;
	uint32_t m_id;

private:
	uint32_t startX;
	uint32_t startY;
};
