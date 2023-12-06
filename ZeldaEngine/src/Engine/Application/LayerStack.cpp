#include "LayerStack.h"   
#include "Layer.h"

namespace Engine {

	std::vector<Layer*>::iterator LayerStack::LayersFront() 
	{
		return m_LayerStack.begin();
	} 

	std::vector<Layer*>::iterator LayerStack::LayersBack() 
	{
		return m_LayerStack.end();
	} 

	std::vector<Layer*>::iterator LayerStack::OverlaysFront()
	{
		return m_OverLayStack.begin();
	} 

	std::vector<Layer*>::iterator LayerStack::OverLaysBack()
	{
		return m_OverLayStack.end();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rLayersFront()
	{
		return m_LayerStack.rbegin();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rLayersBack()
	{
		return m_LayerStack.rend();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rOverlaysFront()
	{
		return m_OverLayStack.rbegin();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rOverLaysBack()
	{
		return m_OverLayStack.rend();
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
