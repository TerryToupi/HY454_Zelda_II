#pragma once  

#include<vector>

namespace Engine 
{ 
	class Layer; 

	class LayerStack
	{  
	public: 
		void pushBackLayer(Layer* layer);
		void pushBackOverLay(Layer* overLay);
		void popBackLayer();
		void popBackOverLay();

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
	};
}

