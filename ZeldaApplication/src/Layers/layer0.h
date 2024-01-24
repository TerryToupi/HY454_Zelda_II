#pragma once 

#include <Engine.h>

using namespace Engine; 

class Layer0 : public Layer 
{ 
public:
	Layer0(); 

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override; 

	void onEvent(Event& e) override;    

	void move();

	bool mover(KeyPressEvent& e);  

private:
	Ref<MovingAnimator> m_CamLeft;
	Ref<MovingAnimator> m_CamRight;

	float wdx = 0;
	float wdy = 0;
	
	Time curr = 0;
};
