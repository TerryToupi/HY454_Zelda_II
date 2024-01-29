#pragma once 

#include <Engine/Application/Core.h>   
#include <Engine/UUID/UUID.h>
#include <SDL.h> 

#include <unordered_map>

namespace Engine
{
	typedef Uint64 AudioID; 
	typedef Uint32 DeviceID;

	struct AudioData
	{  
		AudioID id;
		SDL_AudioSpec wavSpec;
		Uint32 wavLength; 
		Uint8* wavBuffer;
	}; 

	struct MusicData
	{
		DeviceID id;
		bool looping;
		SDL_AudioSpec wavSpec;
		Uint32 wavLength; 
		Uint32 wavLengthConst;
		Uint8* wavBuffer;
		Uint8* wavBufferConst;
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
		using SoundStorage = std::unordered_map<AudioID, AudioData*>;
		using MousicStorage = std::unordered_map<DeviceID, MusicData*>;

	public: 
		AudioID LoadSound(std::string _path);  
		DeviceID InitMusicDevice(std::string _path, bool looping); 
		void PauseMusicDevice(DeviceID _device, bool _v); 
		void DeleteMusicAndDevice(DeviceID _device);
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
		SoundStorage m_sounds; 
		MousicStorage m_tracs;
		DeviceChain* m_itr = nullptr; 
		DeviceChain* m_head = nullptr; 

	protected: 
		friend class Application; 

	private:
		static AudioManager s_instance;
	};
}
