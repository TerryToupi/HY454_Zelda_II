#pragma once 

#include "Engine/Core.h"
#include "LayerStack.h"

namespace Engine {
 
	class Application
	{
	public:
		Application();
		virtual ~Application(); 

		void pushLayer(Layer *layer);  
		void pushOverLay(Layer *Overlay);

		void Run();

	private:
		LayerStack m_Layers;
	};

	Application* CreateApplication();
}
