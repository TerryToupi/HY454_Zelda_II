#pragma once

#include "Engine/Core.h" 
#include <random> 

namespace Engine {
	typedef uint64_t ID; 

	class UUID
	{
	public: 
		UUID();  
		UUID(ID id);
		UUID(const UUID& other) = default; 
		
		operator ID() const { return m_ID; }
	private: 
		ID m_ID;
	};
}
