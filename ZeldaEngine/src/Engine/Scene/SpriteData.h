#pragma once  
#include<Engine/Scene/LatelyDestroyable.h>
#include<Engine/Application/Core.h> 
#include<Engine/Math/Math.h> 
#include<Engine/UUID/UUID.h> 
#include<Engine/Renderer/MotionQuantizer.h> 
//#include<Engine/Renderer/GravityHandler.h> 
#include<Engine/Renderer/Clipper.h>

#include<functional>

namespace Engine
{ 
	class SpriteData : public LatelyDestroyable
	{
	public:
		using Mover = std::function<void(const Rect&, int* dx, int* dy)>;   
	
		byte			frameNo = 0;
		Rect			frameBox;
		int				x = 0, y = 0; 
		bool			isVisible = false;  
		bool			directMotion = false;
		// ANIMATION FILM
		// BoundingArea
		unsigned		zorder = 0; 
		std::string		typeID, stateID; 
		Mover			mover;  
		MotionQuantizer	quantizer; 
		//GravityHandler	gravity;
		
		SpriteData(const std::string& name);
		SpriteData(const std::string& name, ID uuid); 
		SpriteData(const SpriteData& other) = default;

	private: 
		std::string m_Name; 
		UUID m_UUID;
	}; 
}
