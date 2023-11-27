#pragma once  

#include "Engine/UUID/UUID.h"

namespace Engine {

	struct component_id
	{
		UUID uuid; 
		component_id() = default; 
		component_id(const component_id& other) = default;
	};
	
	struct component_tag
	{ 
		std::string tag;  

		component_tag() = default; 
		component_tag(const component_tag& other) = default;
		component_tag(std::string& tag)
		{
			this->tag = tag;
		}
	}; 

	template<typename... Components>
	struct groupComponents { }; 

	using allComponents = 
			groupComponents<component_id, component_tag>;
}
