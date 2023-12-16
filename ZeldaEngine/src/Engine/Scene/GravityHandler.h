#pragma once

#include <Engine/Application/Core.h> 
#include <Engine/Math/Math.h>

namespace Engine
{
	class GravityHandler 
	{
	public:
		using OnSolidGroundPred = std::function<bool(const Rect&)>;
		using OnStartFalling = std::function<void(void)>;
		using OnStopFalling = std::function<void(void)>;

	public:
		template <typename T> void SetOnStartFalling(const OnSolidGroundPred& f)
		{
			onStartFalling = f;
		} 

		template <typename T> void SetOnStopFalling(const T& f)
		{
			onStopFalling = f;
		} 

		template <typename T> void SetOnSolidGround(const T& f)
		{
			onSolidGround = f;
		} 

		void Reset(void) { isFalling = false; }
		void Check(const Rect& r); 

	protected:
		bool gravityAddicted = false;
		bool isFalling = false;
		OnSolidGroundPred onSolidGround;
		OnStartFalling onStartFalling;
		OnStopFalling onStopFalling;
	};
}
