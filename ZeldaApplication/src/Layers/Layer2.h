#pragma once 

#include <Engine.h>
#include "../Link/Link.h"
#include <json.hpp>


using json = nlohmann::json;
using namespace Engine;

using Sheets = std::unordered_map<std::string, AnimationSheet*>;


class Layer2 : public Layer
{
public:
    Layer2();

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override;

	void onEvent(Event& e) override;

	void LoadSheets();

	Link* link;
	Sheets m_overlaySheets;

};