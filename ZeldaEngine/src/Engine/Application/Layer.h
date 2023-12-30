#pragma once

#include <Engine/Application/Core.h> 
#include <Engine/Events/Event.h>  

#include <Engine/Renderer/Bitmap.h>

#include <string> 

namespace Engine {  
	class Layer
	{  
	public: 
		Layer(std::string name);
		virtual ~Layer() = default;

		virtual void onStart() {}
		virtual void onDelete() {}
		virtual void onUpdate() {}     
		virtual void onEvent(Event& e) {}

	private: 
		std::string m_Name;
	};
}

