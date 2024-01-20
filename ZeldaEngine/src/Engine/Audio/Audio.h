#pragma once 

#include <Engine/Application/Core.h> 
#include <SDL.h>

namespace Engine
{
	typedef Uint32 AudioID;

	struct AudioData
	{  
		Uint32 deviceID;
		SDL_AudioSpec wavSpec;
		Uint32 wavLength; 
		Uint8* wavBuffer;
	}; 

	class AudioManager
	{
	public:		
		using DataVector = std::list<AudioData*>; 

	public: 
		Uint32 LoadSound(std::string _path); 
		void PlaySound(int _deviceID); 
		void PauseSound(int _deviceID);   
		void DeleteSound(int _deviceID);

		static auto Get(void) -> AudioManager& { return s_instance; } 
		static auto GetConst(void) -> const AudioManager& { return s_instance; } 

	private:
		AudioManager() = default; 
		AudioManager(const AudioManager& other) = delete; 
		AudioManager(AudioManager&& other) = delete; 
		auto find(int _deviceID) -> std::list<AudioData*>::iterator;

	private:   
		DataVector m_data;

	private:
		static AudioManager s_instance;
	};
}
