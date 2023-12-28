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

	void onUpdate() override; 

	void onEvent(Event& e) override;   

	bool mover(KeyTapEvent& e); 

	bool mover(KeyRepeatEvent& e);
private:
	Reference<Scene> m_Scene; 
	Reference<TileLayer> m_Tiles;	

	float wdx = 0;
	float wdy = 0;
};
