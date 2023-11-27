#pragma once 

#include "LayerStack.h" 
#include <string>

int main(int argc, char** argv);

namespace Engine 
{
	struct ApplicationConfig
	{
		std::string appName;
		std::string WorkingDir;
	};

	class Application
	{
	public:
		Application(const ApplicationConfig& config);
		virtual ~Application(); 

		void pushLayer(Layer *layer);  
		void pushOverLay(Layer *Overlay);

		void Run();

	private: 
		ApplicationConfig m_AppConfig;
		LayerStack m_Layers; 

	private: 
		static Application* s_Instance; 
		friend int ::main(int agc, char** argv);
	};

	Application* CreateApplication();
}
