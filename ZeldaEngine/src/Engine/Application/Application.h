#pragma once 

#include <Engine/Application/LayerStack.h>
#include <Engine/Window/Window.h> 
#include <Engine/Events/ApplicationEvents.h>
#include <Engine/Application/GameTime.h> 
#include <Engine/Scene/LatelyDestroyable.h> 
#include <Engine/Audio/Audio.h>

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

		void popLayer();  
		void popOverLay(); 

		void Run();

	protected: 
		bool OnWindowClose(WindowCloseEvent& e); 
		bool OnWindowResize(WindowResizeEvent& e); 

	private: 
		ApplicationConfig m_AppConfig;
		LayerStack m_Layers;  
		Scope<Window> m_Window;  
		bool m_Running;

	private: 
		static Application* s_Instance; 
	};

	Application* CreateApplication();
}
