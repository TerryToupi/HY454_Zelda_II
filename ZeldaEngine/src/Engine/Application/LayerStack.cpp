#include "LayerStack.h"   
#include "Layer.h"

#include <algorithm>

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

	auto LayerStack::find_layer(Layer* layer) -> std::vector<Layer*>::iterator
	{
		return std::find_if(
			m_LayerStack.begin(),
			m_LayerStack.end(),
			[layer](const Layer* l)
			{
				return layer->m_Name == l->m_Name;
			}
		);
	}

	auto LayerStack::find_overlay(Layer* overlay) -> std::vector<Layer*>::iterator
	{
		return std::find_if(
			m_OverLayStack.begin(),
			m_OverLayStack.end(),
			[overlay](const Layer* o)
			{
				return overlay->m_Name == o->m_Name;
			}
		);
	}

	auto LayerStack::find_by_name(std::string tag, std::vector<Layer*>& stack) -> std::vector<Layer*>::iterator
	{
		return std::find_if(  
				stack.begin(), 
				stack.end(), 
				[tag](const Layer* l) 
				{  
					return l->m_Name == tag;
				}
			); 
	}

	LayerStack::~LayerStack()
	{ 
		for (auto* layer : m_LayerStack)
		{
			layer->onDelete();
			delete layer;
		} 

		for (auto* overlay : m_OverLayStack)
		{
			overlay->onDelete();
			delete overlay;
		}
	}

	void LayerStack::pushBackLayer(Layer* layer)
	{  
		m_LayerStack.emplace_back(layer);
	} 

	void LayerStack::pushBackOverLay(Layer* overLay)
	{  
		m_OverLayStack.emplace_back(overLay);
	} 

	void LayerStack::popBackLayer(Layer* layer)
	{    
		ENGINE_CORE_ASSERT(!m_LayerStack.empty());   

		auto itr = find_layer(layer); 
		(*itr)->onDelete(); 
		m_LayerStack.erase(itr);
		delete layer;
	} 

	void LayerStack::popBackOverLay(Layer* overlay)
	{ 
		ENGINE_CORE_ASSERT(!m_OverLayStack.empty()); 

		auto itr = find_overlay(overlay);
		(*itr)->onDelete();
		m_LayerStack.erase(itr);
		delete overlay;
	} 

	Layer* LayerStack::GetLayer(std::string tag)
	{
		return (*find_by_name(tag, m_LayerStack));
	} 

	Layer* LayerStack::GetOverlay(std::string tag)
	{
		return (*find_by_name(tag, m_OverLayStack));
	}
}
