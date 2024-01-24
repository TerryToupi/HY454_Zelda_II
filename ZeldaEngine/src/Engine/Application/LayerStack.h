#pragma once  

#include<vector>
#include<string>

namespace Engine 
{ 
	class Layer; 

	class LayerStack
	{  
	public:  
		~LayerStack();

		void pushBackLayer(Layer* layer);
		void pushBackOverLay(Layer* overLay);
		void popBackLayer(Layer* layer);
		void popBackOverLay(Layer* overlay);

		Layer* GetLayer(std::string tag);
		Layer* GetOverlay(std::string tag);

		std::vector<Layer*>::iterator LayersFront();
		std::vector<Layer*>::iterator LayersBack();
		std::vector<Layer*>::iterator OverlaysFront();
		std::vector<Layer*>::iterator OverLaysBack(); 

		std::vector<Layer*>::reverse_iterator rLayersFront();
		std::vector<Layer*>::reverse_iterator rLayersBack();
		std::vector<Layer*>::reverse_iterator rOverlaysFront();
		std::vector<Layer*>::reverse_iterator rOverLaysBack();

	private: 
		std::vector<Layer*> m_LayerStack;
		std::vector<Layer*> m_OverLayStack;
		auto find_layer(Layer* layer) -> std::vector<Layer*>::iterator; 
		auto find_overlay(Layer* overlay) -> std::vector<Layer*>::iterator; 
		auto find_by_name(std::string tag, std::vector<Layer*>& stack) -> std::vector<Layer*>::iterator;
	};
}

