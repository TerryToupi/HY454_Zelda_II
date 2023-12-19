#pragma once 

#include <Engine.h>

using namespace Engine; 

class skoupidi : public Layer 
{ 
public:
	skoupidi(); 
	~skoupidi() = default;

	void onAttach();

	void onDelete();

	void onUpdate(); 

	void onEvent(Event& e);  

private: 
};
