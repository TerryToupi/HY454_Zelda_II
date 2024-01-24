#pragma once

#include <Engine/Application/Core.h> 
#include <Engine/Events/Event.h>   
#include <Engine/Application/GameTime.h>
#include <Engine/Scene/Scene.h>

#include <string> 

namespace Engine {   
	class LayerStack;

	class Layer
	{  
	public: 
		Layer(std::string name);
		virtual ~Layer() = default;

		virtual void onStart() {}
		virtual void onDelete() {}
		virtual void onUpdate(Time ts) {}     
		virtual void onEvent(Event& e) {} 

	public:
		Ref<Scene> m_Scene; 

	protected:
		std::string m_Name;  

	protected:
		friend class LayerStack;
	};
}

