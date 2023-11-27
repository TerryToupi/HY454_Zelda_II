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

		std::vector<Layer*>::iterator stackFront();
		std::vector<Layer*>::iterator stackBack();

	private: 
		std::vector<Layer*> m_LayerStack;
		std::vector<Layer*> m_OverLayStack;
	};
}

