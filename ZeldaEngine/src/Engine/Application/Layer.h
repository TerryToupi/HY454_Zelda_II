#pragma once

#include <string>

namespace Engine {  
	class Scene;

	class Layer
	{  
	public: 
		Layer(std::string name);
		virtual ~Layer() = default;

		void virtual onAttach() {}
		void virtual onDettach() {}
		void virtual onUpdate() {}  

	public: 
		Scene* m_Scene; 

	private: 
		std::string m_Name;
	};
}

