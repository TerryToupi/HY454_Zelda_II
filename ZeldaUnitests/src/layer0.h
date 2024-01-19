#pragma once 

#include <Engine.h>

using namespace Engine; 

class layer0 : public Layer 
{ 
public:
	layer0(); 
	~layer0() = default;

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override; 

	void onEvent(Event& e) override;    

	void move();

	bool mover(KeyPressEvent& e); 
private:
	Ref<Scene> m_Scene; 	

	float wdx = 0;
	float wdy = 0;
};
