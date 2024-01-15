#include "Gravity.h"

namespace Engine
{
	void GravityHandler::SetOnSolidGround(const OnSolidGroundPred& f)
	{ 
		m_onSolidGround = f;
	} 

	void GravityHandler::SetOnStartFalling(const OnStartFalling& f)
	{ 
		m_onStartFalling = f;
	}

	void GravityHandler::SetOnStopFalling(const OnStopFalling& f)
	{ 
		m_onStopFalling = f;
	}

	void GravityHandler::SetGravityAddicted(bool _adicted)
	{
		m_gravityAddicted = _adicted;
	}

	void GravityHandler::Reset(void)
	{ 
		m_isFalling = false;
	}

	void GravityHandler::Check(Rect& r)
	{ 
		if (m_gravityAddicted)
		{
			if (m_onSolidGround(r))
			{
				if (m_isFalling)
				{
					m_isFalling = false; 
					m_onStopFalling();
				}
			} 
			else
			{
				if (!m_isFalling)
				{
					m_isFalling = true;  
					m_onStartFalling();
				}
			}
		}
	}
}
