#pragma once  
#include<Engine/Application/Core.h>   
#include<Engine/Scene/LatelyDestroyable.h>
#include<string>

namespace Engine
{ 
	class Animation : public LatelyDestroyable
	{
	public: 
		Animation(const std::string& _id);

		const std::string& GetId(void);
		void			   SetId(const std::string& _id); 
		virtual Animation* Clone(void) const = 0; 

	protected: 
		std::string m_Id;
	};
}
