#pragma once 

#include <Engine/Application/Core.h>   
#include <Engine/UUID/UUID.h>
#include <SDL.h> 

#include <unordered_map>

namespace Engine
{
	typedef Uint64 AudioID;

	struct AudioData
	{  
		AudioID id;
		SDL_AudioSpec wavSpec;
		Uint32 wavLength; 
		Uint8* wavBuffer;
	};  

	struct DeviceChain
	{
		Uint32 deviceID;
		DeviceChain* next;
	}; 

	class Application;

	class AudioManager
	{
	public:		
		using DataStorage = std::unordered_map<Uint32, AudioData*>;

	public: 
		AudioID LoadSound(std::string _path); 
		void PlaySound(AudioID _id); 
		void DeleteSound(AudioID _id);

		static auto Get(void) -> AudioManager& { return s_instance; } 
		static auto GetConst(void) -> const AudioManager& { return s_instance; }  

	protected:
		void Init(); 
		void Shutdown();

	private:
		AudioManager() = default; 
		AudioManager(const AudioManager& other) = delete; 
		AudioManager(AudioManager&& other) = delete; 

	private:   
		DataStorage m_data; 
		DeviceChain* m_itr = nullptr; 
		DeviceChain* m_head = nullptr; 

	protected: 
		friend class Application; 

	private:
		static AudioManager s_instance;
	};
}
