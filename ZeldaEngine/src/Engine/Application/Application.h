#pragma once 

#include <Engine/Application/LayerStack.h>
#include <Engine/Window/Window.h>
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

		void onEvent(Event& e);

		Window& GetWindow() { return *m_Window; }

		static Application& Instance() { return *s_Instance; }

		void pushLayer(Layer *layer);  
		void pushOverLay(Layer *Overlay);

		void Run();

	private: 
		ApplicationConfig m_AppConfig;
		LayerStack m_Layers;  
		Scope<Window> m_Window; 
		bool m_Running;

	private: 
		static Application* s_Instance; 
		friend int ::main(int agc, char** argv);
	};

	Application* CreateApplication();
}
