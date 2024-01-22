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
		using PauseAction = std::function<void(void)>; 

	public:
		Application(const ApplicationConfig& config);
		virtual ~Application(); 

		void onEvent(Event& e);

		Window& GetWindow() { return *m_Window; }

		static Application& Instance() { return *s_Instance; }

		void static pushLayer(Layer *layer);  
		void static pushOverLay(Layer *Overlay);

		void static popLayer();  
		void static popOverLay(); 

		void Run();

	protected:   
		void SetOnPauseFunction(const PauseAction& f);
		void SetOnResumeFunction(const PauseAction& f);
		void Pause(Time t);
		void Resume(void);
		bool IsPaused(void);
		Time GetPauseTime(void) const;
	 
		bool OnWindowClose(WindowCloseEvent& e); 
		bool OnWindowResize(WindowResizeEvent& e);  
		bool OnWindowPause(WindowPauseEvent& e);

	private:  
		ApplicationConfig m_AppConfig;
		LayerStack m_Layers;  
		Scope<Window> m_Window;  
		PauseAction m_pauseFunction;  
		PauseAction m_resumeFunction;
		Time m_pauseTimestamp;
		bool m_Running = false; 
		bool m_isPaused = false;

	private: 
		static Application* s_Instance; 
	};

	Application* CreateApplication();
} 

