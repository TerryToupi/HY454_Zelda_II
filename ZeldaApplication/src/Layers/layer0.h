#pragma once 

#include <Engine.h>
#include "../Entity/Entity.h"
#include "../Misc/Clouds/Cloud.h"
#include "layer1.h"

using namespace Engine; 

class Layer0 : public Layer 
{ 
public:
	using Clouds = std::unordered_map<uint32_t, Cloud*>;
	using Sheets = std::unordered_map<std::string, AnimationSheet*>;
	
	Layer0(); 
	
	/*-----INITIALIZERS-----*/
	void InitializeCloudData();
	void InitializeClouds();

	/*----LAYER FUNCTIONS----*/
	void onStart() override;
	void onDelete() override;
	void onUpdate(Time ts) override; 
	void onEvent(Event& e) override; 
	bool MoveSky(Event& e);
	bool StopSky(Event& e);


	/*----HANDLERS---*/
	void CloudHandler();
	void SpawnCloud();

private:
	Ref<MovingAnimator> m_CamLeft;
	Ref<MovingAnimator> m_CamRight;

	std::vector<uint32_t> m_cloudY;
	std::vector<uint32_t> m_speeds;
	Clouds m_clouds;

	Sheets m_sheets;

	float wdx = 0;
	float wdy = 0;

};
