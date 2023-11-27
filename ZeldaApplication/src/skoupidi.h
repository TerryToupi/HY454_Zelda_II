#pragma once 

#include <Engine.h>

using namespace Engine; 

class skoupidi : public Layer 
{ 
public:
	skoupidi(); 
	~skoupidi() = default;

	void onAttach();

	void onDettach();

	void onUpdate();
};
