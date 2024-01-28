#pragma once

#pragma once 

#include <Engine.h>
#include "../Link/Link.h"
#include <json.hpp>

using namespace Engine;

using Sheets = std::unordered_map<std::string, AnimationSheet*>;


class Overlay : public Layer
{
public:
	Overlay();

	void onStart() override;

	void onDelete() override;

	void onUpdate(Time ts) override;

	void onEvent(Event& e) override;

	AnimationSheet* m_sheet;
	AnimationFilm* m_film;

};
