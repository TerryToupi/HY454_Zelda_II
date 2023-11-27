#include "LayerStack.h"   
#include "Layer.h"

namespace Engine {

	std::vector<Layer*>::iterator LayerStack::stackFront() 
	{
		return m_LayerStack.begin();
	} 

	std::vector<Layer*>::iterator LayerStack::stackBack() 
	{
		return m_LayerStack.end();
	} 

	void LayerStack::pushBackLayer(Layer* layer)
	{ 
		layer->onAttach();
		m_LayerStack.emplace_back(layer);
	} 

	void LayerStack::pushBackOverLay(Layer* overLay)
	{  
		overLay->onAttach();
		m_OverLayStack.emplace_back(overLay);
	} 

	void LayerStack::popBackLayer()
	{ 
		m_LayerStack.pop_back();
	} 

	void LayerStack::popBackOverLay()
	{ 
		m_OverLayStack.pop_back();
	}
}
