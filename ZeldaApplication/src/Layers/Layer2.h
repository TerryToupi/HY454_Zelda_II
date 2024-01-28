#pragma once 

#include <Engine.h>
#include "../Link/Link.h"
#include <json.hpp>


using json = nlohmann::json;
using namespace Engine;

class Layer2 : public Layer
{
public:
    Layer2();

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override;

	void onEvent(Event& e) override;

	void LoadSheets();

	AnimationSheet* m_sheet;
	AnimationFilm* m_film;
};