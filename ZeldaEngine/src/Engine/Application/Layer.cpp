#include "Layer.h"

namespace Engine {
	Layer::Layer(std::string name)
	{ 
		this->m_Name = name; 
		this->m_Scene = nullptr;
	}
}
