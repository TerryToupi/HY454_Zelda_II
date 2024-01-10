#pragma once 

#include <Engine.h>

using namespace Engine;

class layer1 : public Layer
{
public:
	layer1();
	~layer1() = default;

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override;

	void onEvent(Event& e) override;

	bool mover(Event& e);  

	static void FrameRangeActionLeft(layer1& layer);
	static void FrameRangeActionRight(layer1& layer);

	void move(Time ts); 

private: 
	Ref<Scene> m_Scene;  
	Ref<AnimationSheet> m_linkSheet; 

	Ref<AnimationFilm> m_WalkRight;
	Ref<AnimationFilm> m_WalkLeft; 
	Ref<FrameRangeAnimation> m_walkRightAnim;
	Ref<FrameRangeAnimation> m_walkLeftAnim; 

	Ref<FrameRangeAnimator> m_animator1;
	Ref<FrameRangeAnimator> m_animator2;

	float wdx = 0; 
	float wdy = 0; 

	Time curr = 0;
};