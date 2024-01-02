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

	void onUpdate() override;

	void onEvent(Event& e) override;

	bool mover(KeyPressEvent& e); 

	void move();
private:
	Reference<Scene> m_Scene;

	float wdx = 0;
	float wdy = 0;
};