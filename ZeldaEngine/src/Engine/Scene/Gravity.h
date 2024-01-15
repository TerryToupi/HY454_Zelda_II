#pragma once 

#include <Engine/Application/Core.h>  
#include <Engine/Math/Math.h>

namespace Engine
{
	class GravityHandler
	{
	public:
		using OnSolidGroundPred = std::function<bool(Rect&)>; 
		using OnStartFalling = std::function<void(void)>; 
		using OnStopFalling = std::function<void(void)>;

	public:
		void SetOnSolidGround(const OnSolidGroundPred& f);
		void SetOnStartFalling(const OnStartFalling& f); 
		void SetOnStopFalling(const OnStopFalling& f);
		void SetGravityAddicted(bool _adicted);
		void Reset(void);
		void Check(Rect& r);

	protected:
		bool m_gravityAddicted = false;
		bool m_isFalling = false;
		OnSolidGroundPred m_onSolidGround;
		OnStartFalling m_onStartFalling;
		OnStopFalling m_onStopFalling;
	};
}
