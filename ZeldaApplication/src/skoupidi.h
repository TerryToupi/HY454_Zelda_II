#pragma once 

#include <Engine.h>

using namespace Engine; 

class skoupidi : public Layer 
{ 
public:
	skoupidi(); 
	~skoupidi() = default;

	void onStart() override;

	void onDelete() override;

	void onUpdate() override; 

	void onEvent(Event& e) override;   

	bool function(KeyTapEvent& e);

private:
	Reference<Scene> m_Scene; 
	Reference<TileLayer> m_Tiles;
};
