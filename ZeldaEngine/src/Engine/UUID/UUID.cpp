#include "UUID.h" 

namespace Engine { 

	static std::random_device s_RandomDevice; 
	static std::mt19937_64 s_Engine(s_RandomDevice()); 
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;  

	UUID::UUID()
	{ 
		this->m_ID = s_UniformDistribution(s_Engine);
	}  

	UUID::UUID(ID id) 
	{
		this->m_ID = id;
	} 

	ID UUID::GenerateUUID()
	{
		return s_UniformDistribution(s_Engine);
	}
};
