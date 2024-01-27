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
	void EraseAnimation(Animation* animation);

	void SetSprite(Sprite s);
	void LeftAttackPosUpdate(std::string name);

	void SetState(std::string _state);
	void SetLookingAt(std::string _loookingAt);
	void SetSheet(AnimationSheet* _sheet);
	std::string GetState();
	std::string GetLookingAt();
	Animators GetAnimators();
	uint32_t GetID() const;

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
	uint32_t m_id;

	Ref<Scene> m_scene;

private:
	uint32_t startX;
	uint32_t startY;
};
